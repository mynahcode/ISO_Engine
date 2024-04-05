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
		void CreateTileGrid(const glm::uvec2& gridSize, const glm::vec2& tileSize, int level);

	private:
		void UpdateLightMap(const std::vector<Entity>& tiles, int threshold, int range, int depth_n, int count, int lightType);
		void UpdateTile(const std::vector<Entity>& tiles, const std::pair<int, int>& tileIndex, int tilesPerDepth, int lightType, int threshold, int range);

		glm::uvec2 m_GridSize;
		glm::vec2 m_TileScale;
		glm::vec2 m_GridOrigin;
		std::map<int, std::vector<Entity>> m_TileGrids;
		std::map<int, std::vector<float>> m_LightMaps;

		// BFS
		int tilesPerDepth;
		int invDiagCount;
		bool invSwitch;
		std::pair<int, int> m_OriginCoords;
		std::queue<std::pair<int, int>> m_TilesToExplore;
		std::vector<std::pair<int,int>> m_TilesInQueue;
		std::unordered_set<uint32_t> m_ComputedLights;

		Ref<Scene> m_ActiveScene;
	};
}