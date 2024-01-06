#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <IsoEngine.h>

namespace IE
{
	class GridManager
	{
	public:

		//GridManager(const glm::uvec2& gridSize, const glm::vec2& tileSize, bool isIsometric);
		~GridManager() = default;



	private:
		std::vector<std::vector<Entity>> m_TileGrids;
		bool IsIsometric;
	};
}