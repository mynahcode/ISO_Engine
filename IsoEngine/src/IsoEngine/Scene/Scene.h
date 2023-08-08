#pragma once

#include "IsoEngine/Core/Timestep.h"

#include <entt.hpp>

namespace IE
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();
		// Temporary
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry; // Contains entity component data and entity IDs --> Container that contains entity "context"
	};
}