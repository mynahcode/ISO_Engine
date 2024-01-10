#pragma once

#include <map>
#include <vector>
#include <glm/glm.hpp>

#include <IsoEngine.h>

namespace IE
{
	class GridManager
	{
	public:

		GridManager() = default;
		GridManager(Ref<Scene> scene, const glm::uvec2& gridSize, const glm::vec2& tileScale, const glm::vec2& origin);
		~GridManager() = default;

		void AddGridLevel(int zLevel);
		void RemoveGridLevel(int zLevel);

	private:
		void CreateTileGrid(const glm::uvec2& gridSize, const glm::vec2& tileSize, int level);

		glm::uvec2 m_GridSize;
		glm::vec2 m_TileScale;
		glm::vec2 m_GridOrigin;
		std::map<int, std::vector<Entity>> m_TileGrids;

		Ref<Scene> m_ActiveScene;

		friend class Entity;
	};
}