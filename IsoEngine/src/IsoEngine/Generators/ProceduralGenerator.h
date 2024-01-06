#pragma once

#include "IsoEngine/Scene/Entity.h"

namespace IE
{
	class ProceduralGenerator
	{
	public:
		ProceduralGenerator(const std::vector<Entity>& tileGrid);

		~ProceduralGenerator() = default;

	private:
		std::vector<Entity> m_TileGrid;
	};
}