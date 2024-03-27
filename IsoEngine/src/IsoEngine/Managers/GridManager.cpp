#include "iepch.h"

#include "GridManager.h"
#include "IsoEngine/Scene/Components.h"

namespace IE
{
	GridManager::GridManager(Ref<Scene> scene, const glm::uvec2& gridSize, const glm::vec2& tileScale, const glm::vec2& origin)
		: m_ActiveScene(scene), m_GridSize(gridSize), m_TileScale(tileScale), m_GridOrigin(origin)
	{
        CreateTileGrid(m_GridSize, m_TileScale, 0);
	}

    void GridManager::AddGridLevel(int zLevel)
    {
        CreateTileGrid(m_GridSize, m_TileScale, zLevel);
    }

    void GridManager::RemoveGridLevel(int zLevel)
    {
        auto iter = m_TileGrids.find(zLevel);
        m_TileGrids.erase(iter);
    }

    // TODO: Move to a separate class for handling lights.
    void GridManager::CreateLightMap(int zLevel)
    {
        auto convert2DCoord = [&](int index)
        {
            return std::pair
            {
                (index / m_GridSize.y),
                (index % m_GridSize.y)
            };
        };
        //ISOLOGGER_CRITICAL("Recreating Light Map at Z-Level {0}\n", zLevel);
        std::vector tileGrid = m_TileGrids[zLevel];
        for (auto& tileEntity : tileGrid)
        {
            auto it = std::find(tileGrid.begin(), tileGrid.end(), tileEntity);
            int index = it - tileGrid.begin(); // Guaranteed to be in vector
            if (tileEntity.HasComponent<LightComponent>() && m_ComputedLights.find(index) == m_ComputedLights.end()) // To prevent recalculating same tileLight.
            {
                int lightRange = tileEntity.GetComponent<LightComponent>().Range;

                m_TilesToExplore = {};
                m_TilesInQueue = {};
                m_TilesToExplore.push(convert2DCoord(index));
                m_TilesInQueue.push_back(convert2DCoord(index));
                int depth_n = 1;
                int count = 0;
                UpdateLightMap(tileGrid, lightRange, lightRange, depth_n, count);

                m_ComputedLights.insert(index);
            }
        }
    }

    void GridManager::UpdateLightMap(const std::vector<Entity>& tiles, int threshold, int range, int depth_n, int count)
    {
        auto convert1DCoord = [&](int x, int y)
        {
            return uint32_t
            {
                (y + ((m_GridSize.y) * x))
            };
        };

        auto convert2DCoord = [&](int index)
        {
            return std::pair
            {
                (index / m_GridSize.y),
                (index % m_GridSize.y)
            };
        };

        auto addTileToExplore = [&](int x, int y)
        {
            std::pair tileIndex = std::make_pair(x, y);
            if (std::find(m_TilesInQueue.begin(), m_TilesInQueue.end(), tileIndex) == m_TilesInQueue.end())
            {
                //ISOLOGGER_TRACE("Tile To Explore At Threshold Level and depth_n: {0}, {1}; at Tile Grid index: < {2} > \n", threshold, depth_n, index);
                m_TilesInQueue.push_back(tileIndex);
                m_TilesToExplore.push(tileIndex);
            }
        };
        
        int tilesPerDepth = ((2 * depth_n) + 1) + (2 * (depth_n - 1) + 1);
        if (!m_TilesToExplore.empty())
        {
            ISOLOGGER_CRITICAL("Count: {0}\n", count);
            auto tileIndex = m_TilesToExplore.front();
            m_TilesToExplore.pop();
            ISOLOGGER_TRACE("Exploring tile at index: <{0}, {1}>\n", tileIndex.first, tileIndex.second);
            if ((tileIndex.first >= 0 && tileIndex.second >= 0) && (tileIndex.first < m_GridSize.x && tileIndex.second < m_GridSize.y)) // if valid index, update tile light
            {
                auto index = convert1DCoord(tileIndex.first, tileIndex.second);
                auto tileIter = *(tiles.begin() + index); // Guaranteed to be in vector
                auto& tileLightLevel = tileIter.GetComponent<SpriteRendererComponent>().LightLevel;
                //ISOLOGGER_TRACE("Exploring tile at index: {0}\n", tileIndex);

                float linearFactor = ((float)threshold / range);
                tileLightLevel += linearFactor;
                if (tileLightLevel > 1.0f)
                    tileLightLevel = 1.0f;
            }
            if (tilesPerDepth == count)
            {
                ISOLOGGER_INFO("Incrementing depth_n\n");
                count = 0;
                depth_n++;
                threshold--;
            }
            count++;
            if (threshold > 0)
            {
                addTileToExplore(tileIndex.first, tileIndex.second + 1); // top
                addTileToExplore(tileIndex.first + 1, tileIndex.second); // right
                addTileToExplore(tileIndex.first, tileIndex.second - 1); // bottom
                addTileToExplore(tileIndex.first - 1, tileIndex.second); // left
                UpdateLightMap(tiles, threshold, range, depth_n, count);
            }
        }
        //ISOLOGGER_CRITICAL("LIGHTMAP CALCULATION FINISHED \n");
    }

	void GridManager::CreateTileGrid(const glm::uvec2& gridSize, const glm::vec2& tileSize, int zLevel)
	{
        std::vector<Entity> m_TileGrid;
        // lambda func for ToScreen
        // converts 2D coords to isometric
        /*
        auto ToScreen = [&](float x, float y)
        {
            return glm::vec2
            {
                (m_GridOrigin.x * tileSize.x) + (x - y) * (tileSize.x / 2) - (1.0f * tileSize.x),
                (m_GridOrigin.y * tileSize.y) + (x + y) * (tileSize.y / 2) - (1.0f * tileSize.y)
            };
        };
        */
        for (uint64_t j = 0; j < gridSize.y; j++)
        {
            for (uint64_t i = 0; i < gridSize.y; i++)
            {
                glm::vec3 tilePosition = { (float)i, (float)j, (float)zLevel }; // {x, y, z}
                //auto spriteTexture = m_ActiveScene->GetSpriteTexture(0); // 6, 11, 14 
                Entity tileEntity = m_ActiveScene->CreateTileEntity(tileSize, tilePosition, m_ActiveScene->GetSpriteTexture(7)); //def: 13
                m_TileGrid.push_back(tileEntity);
            }
        }

        m_TileGrids[zLevel] = m_TileGrid;
	}
}