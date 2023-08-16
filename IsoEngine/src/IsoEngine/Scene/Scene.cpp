#include "iepch.h"
#include "Scene.h"
#include "Components.h"
#include "IsoEngine/Renderer/Renderer2D.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace IE
{
	/*
	static void OnTransformConstruct(entt::registry& registry, entt::entity entity) // takes in reference to registry as well as entity component created on
	{

	}
	*/
	Scene::Scene()
	{
		/* Example Code ECS
		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		
		if(m_Registry.any_of<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

		// Iterate
		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}
		*/
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>(); // ALL entties must have a TransformComponent.
		auto& entity_tag = entity.AddComponent<TagComponent>();
		entity_tag.Tag = name.empty() ? "Unnamed Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>); // Or a sprite 2D
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}