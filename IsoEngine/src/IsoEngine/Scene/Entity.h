#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace IE
{
	// Wrapper for entt
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity e_Handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename ...Args>
		T& AddComponent(Args&&... args)
		{
			IE_ENGINE_ASSERT(!HasComponent<T>(), "Entity already has component!")
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			IE_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have component on GetComponent call!")
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			IE_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have component already on RemoveComponent call!")
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_EntityHandle = { entt::null }; // entt::null and entt 0 are not the same.
		//std::weak_ref<Scene> m_Scene; // TODO: Refence counting.
		Scene* m_Scene = nullptr; 
	};
}