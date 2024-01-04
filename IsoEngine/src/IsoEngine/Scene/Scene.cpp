#include "iepch.h"
#include "Scene.h"

#include "Components.h"
#include "IsoEngine/Renderer/Renderer2D.h"
#include "Entity.h"
#include <glm/glm.hpp>

namespace IE
{
	Scene::Scene()
	{
		LoadBoundedTextures();
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>(); // ALL entities must have a TransformComponent.
		auto& entity_tag = entity.AddComponent<TagComponent>();
		entity_tag.Tag = name.empty() ? "Unnamed Entity" : name;
		return entity;
	}

	Entity Scene::CreateTileEntity(const glm::vec2& dimensions, const glm::vec3& position)
	{
		auto tileEntity = CreateEntity("Tile");
		auto& entity_transform = tileEntity.GetComponent<TransformComponent>();
		entity_transform.Translation = position;
		tileEntity.AddComponent<TileComponent>(dimensions);
		tileEntity.AddComponent<SpriteRendererComponent>(m_BoundedTextures[1]); // pos 1 <- default emptyTile texture
		return tileEntity;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.isPrimary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::LoadBoundedTextures()
	{
		// TODO: Implement
		/*
		for (int i = 0; i < 32; i++)
		{
			if (i == 0)
			{
				m_Texture = nullptr;
				m_BoundedTextures.push_back(m_Texture);
				continue;
			}
			
			m_Texture = m_SceneTextures[i];
		}
		*/
		
		// Temporary:
		// Textures[] <- {blank, emptyTile, selectTile, rocky1, rocky2, sandy1, roughconrete1, grass}
		// Indices[] <- {0     , 1         , 2        ,3       ,4      ,5      ,6             ,7}
		m_BoundedTextures.push_back(Ref<Textures2D>(Textures2D::Create(""))); // for the blank texture bounded slot 0
		m_BoundedTextures.push_back(Ref<Textures2D>(Textures2D::Create("assets/textures/emptytile_white.png")));
		m_BoundedTextures.push_back(Ref<Textures2D>(Textures2D::Create("assets/textures/select.png")));
		m_BoundedTextures.push_back(Ref<Textures2D>(Textures2D::Create("assets/textures/rockyground01.png")));
		m_BoundedTextures.push_back(Ref<Textures2D>(Textures2D::Create("assets/textures/rockyground02.png")));
		m_BoundedTextures.push_back(Ref<Textures2D>(Textures2D::Create("assets/textures/sandyground01.png")));
		m_BoundedTextures.push_back(Ref<Textures2D>(Textures2D::Create("assets/textures/roughconcrete01.png")));
		m_BoundedTextures.push_back(Ref<Textures2D>(Textures2D::Create("assets/textures/grass.jpg")));
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// TODO: Move to Scene::OnScenePlay and Scene::OnSceneStop -> calls OnDestroy()
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };

						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.isPrimary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.GetTransform();
					break;
				}
			}
		}
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>); // Or a sprite 2D
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				// TODO: Check if color or texture or both
				//Renderer2D::DrawQuad(transform, sprite.Texture, 1, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, const PerspectiveEditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		// Tilemap rendering
		auto tileGroup = m_Registry.group<TransformComponent>(entt::get<TileComponent, SpriteRendererComponent>);
		for (auto tileEntity : tileGroup)
		{
			auto [tileTransform, tileComponent, tileSprite] = tileGroup.get<TransformComponent, TileComponent, SpriteRendererComponent>(tileEntity);
			Renderer2D::DrawQuad(tileTransform.GetTransform(), tileSprite.Texture, 1.0f, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		/*
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>); // Or a sprite 2D
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			// TODO: Check if color or texture or both
			Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, 1.0f, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
			//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
		}
		*/
		Renderer2D::EndScene();
	}

	void Scene::OnUpdateEditor(Timestep ts, const OrthographicCameraController& camera)
	{
		Renderer2D::BeginScene(camera.GetCamera());
		// Tilemap rendering
		auto tileGroup = m_Registry.group<TransformComponent>(entt::get<TileComponent, SpriteRendererComponent>);
		for (auto tileEntity : tileGroup)
		{
			auto [tileTransform, tileComponent, tileSprite] = tileGroup.get<TransformComponent, TileComponent, SpriteRendererComponent>(tileEntity);
			if (IsometricProjection)
			{
				glm::vec2 size = { 1.0f, 1.0f };
				Renderer2D::DrawRotatedQuad(tileTransform.Translation, size, -45.0f, tileSprite.Texture, 1.0f, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
			}
		}

		/*
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>); // Or a sprite 2D
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			// TODO: Check if color or texture or both
			Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, 1.0f, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
			//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
		}
		*/
		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}

	void Scene::RenderScene(Ref<Framebuffer> fb)
	{
		uint32_t fbTextureColor = fb->GetColorAttachmentRendererID();
		auto screenview = m_Registry.view<ScreenQuadComponent>();
		for (auto ent : screenview)
		{
			auto& screenQuad = screenview.get<ScreenQuadComponent>(ent);
			screenQuad.ScreenTexture = Textures2D::Create(screenQuad.Width, screenQuad.Height, fbTextureColor);
			Renderer2D::DrawQuad(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec2{ screenQuad.Width, screenQuad.Height },
				screenQuad.ScreenTexture, 1.0f, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
		}
		Renderer2D::Draw(fbTextureColor);
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	// Template specialization
	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<ScreenQuadComponent>(Entity entity, ScreenQuadComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TileComponent>(Entity entity, TileComponent& component)
	{

	}

}
