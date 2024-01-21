#pragma once

#include "IsoEngine/Core/Timestep.h"
#include "IsoEngine/Controllers/OrthographicCameraController.h"
#include "IsoEngine/Renderer/PerspectiveEditorCamera.h"
#include "IsoEngine/Renderer/Framebuffer.h"
#include "IsoEngine/Renderer/Textures.h"
#include "IsoEngine/Renderer/SubTexture2D.h"

#include <entt.hpp>
#include <vector>


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
		Entity CreateTileEntity(const glm::vec2& dimensions, const glm::vec3& position, const Ref<SubTexture2D>& spriteTexture);

		Entity GetPrimaryCameraEntity();
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, const PerspectiveEditorCamera& camera);
		void OnUpdateEditor(Timestep ts, const OrthographicCameraController& camera);
		void RenderScene(Ref<Framebuffer> fb);
		void OnViewportResize(uint32_t width, uint32_t height);

		void LoadBoundedTextures();
		void AddSpriteSheetTextures(const std::vector<Ref<SubTexture2D>>& sprites);

		Ref<SubTexture2D> GetSpriteTexture(int index) const { return m_SpriteSheetTextures[index]; }

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		Ref<OrthographicCameraController> m_CameraController;

		entt::registry m_Registry; // Contains entity component data and entity IDs --> Container that contains entity "context"
		uint32_t m_ViewportWidth = 1920, m_ViewportHeight = 1080;

		bool IsometricProjection = true;

		std::vector<Ref<Textures2D>> m_BoundedTextures;
		std::vector<Ref<SubTexture2D>> m_SpriteSheetTextures;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}