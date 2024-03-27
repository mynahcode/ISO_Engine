#pragma once

#include <map>
#include <vector>
#include <glm/glm.hpp>

#include "IsoEngine/Scene/Entity.h"

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

		void CreateLightMap(int zLevel);

	private:
		void CreateTileGrid(const glm::uvec2& gridSize, const glm::vec2& tileSize, int level);
		void UpdateLightMap(const std::vector<Entity>& tiles, int threshold, int range, int depth_n, int count);

		glm::uvec2 m_GridSize;
		glm::vec2 m_TileScale;
		glm::vec2 m_GridOrigin;
		std::map<int, std::vector<Entity>> m_TileGrids;
		std::map<int, std::vector<float>> m_LightMaps;

		// BFS
		std::queue<std::pair<int, int>> m_TilesToExplore;
		std::vector<std::pair<int,int>> m_TilesInQueue;
		std::unordered_set<uint32_t> m_ComputedLights;

		Ref<Scene> m_ActiveScene;
	};
}