#pragma once

#include "IsoEngine/Core/Timestep.h"

#include <entt.hpp>

namespace IE
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry; // Contains entity component data and entity IDs --> Container that contains entity "context"
		friend class Entity;
	};
}