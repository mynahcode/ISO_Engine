#pragma once

#include "IsoEngine/Core/Timestep.h"
#include "IsoEngine/Controllers/OrthographicCameraController.h"
#include "IsoEngine/Renderer/PerspectiveEditorCamera.h"
#include "IsoEngine/Renderer/Framebuffer.h"

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
		Entity CreateTileEntity(const glm::vec2& dimensions, const glm::vec3& position);

		Entity GetPrimaryCameraEntity();
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, PerspectiveEditorCamera& camera);
		void OnUpdateEditor(Timestep ts, OrthographicCameraController& camera);
		void RenderScene(Ref<Framebuffer> fb);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		Ref<OrthographicCameraController> m_CameraController;

		entt::registry m_Registry; // Contains entity component data and entity IDs --> Container that contains entity "context"
		uint32_t m_ViewportWidth = 1920, m_ViewportHeight = 1080;

		bool IsometricProjection = true;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}