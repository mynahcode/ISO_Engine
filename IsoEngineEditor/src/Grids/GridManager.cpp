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
                (m_GridOrigin.y * tileSize.y) + (x + y) * (tileSize.y / 2) - (1.0f * tileSize.y)
                                                                             //0.5f - (1.0f * tileSize.y) - (1.0f * tileSize.x)
            };
        };

        for (uint64_t j = 0; j < gridSize.y; j++)
        {
            for (uint64_t i = 0; i < gridSize.x; i++)
            {
                glm::vec3 tilePosition = { (float)i, (float)j, (float)zLevel }; // {x, y, z}
                //auto spriteTexture = m_ActiveScene->GetSpriteTexture(0); // 6, 11, 14 
                Entity tileEntity = m_ActiveScene->CreateTileEntity(tileSize, tilePosition, m_ActiveScene->GetSpriteTexture(13), m_ActiveScene->GetSpriteTexture(5));
                m_TileGrid.push_back(tileEntity);
            }
        }

        m_TileGrids[zLevel] = m_TileGrid;
	}
}