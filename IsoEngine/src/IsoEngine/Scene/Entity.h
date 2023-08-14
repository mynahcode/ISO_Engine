#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace IE
{
	// Wrapper for entt
	class Entity
	{
	public:
		Entity(entt::entity e_Handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool hasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

	private:
		entt::entity m_EntityHandle;
		//std::weak_ref<Scene> m_Scene; // TODO: Refence counting.
		Scene* m_Scene; 
	};
}