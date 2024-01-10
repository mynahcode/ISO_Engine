#include "iepch.h"

#include "GridManager.h"

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

	void GridManager::CreateTileGrid(const glm::uvec2& gridSize, const glm::vec2& tileSize, int zLevel)
	{
        std::vector<Entity> m_TileGrid;
        // lambda func for ToScreen
        // converts 2D coords to isometric
        auto ToScreen = [&](float x, float y)
        {
            return glm::vec2
            {
                (m_GridOrigin.x * tileSize.x) + (x - y) * (tileSize.x / 2) - (1.0f * tileSize.x),
                (m_GridOrigin.y * tileSize.y) + (y + x) * (tileSize.y / 2) - (0.5f * tileSize.y)
            };
        };

        for (uint64_t j = 0; j < gridSize.y; j++)
        {
            for (uint64_t i = 0; i < gridSize.x; i++)
            {
                glm::vec3 tilePosition = { ToScreen(i, j), (float)zLevel }; // {x, y, z}
                //ISOLOGGER_WARN("Creating Tile at position:< {0}, {1}> \n", tilePosition.x, tilePosition.y);
                Entity tileEntity = m_ActiveScene->CreateTileEntity(tileSize, tilePosition);
                m_TileGrid.push_back(tileEntity);
            }
        }

        m_TileGrids[zLevel] = m_TileGrid;
	}
}