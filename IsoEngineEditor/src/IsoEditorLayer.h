#pragma once

#include <vector>

#include <IsoEngine.h>
#include "Panels/SceneHierarchyPanel.h"

namespace IE
{

	class IsoEditorLayer : public Layer
	{
	public:
		IsoEditorLayer();
		virtual ~IsoEditorLayer() = default;

		/* IsoEngine Layer Class Functions */
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		/* IsoEngine Application Functions */
		void OnUpdate(Timestep timestep) override;
		void OnEvent(Event& ev) override;

		void CreateTileGrid(const glm::uvec2& gridSize, const glm::vec2& tileSize);

	private:
		void NewScene();
		void LoadScene();
		void SaveSceneAs();
		bool OnKeyPressed(KeyPressedEvent& e);

		OrthographicCameraController m_CameraController;
		PerspectiveEditorCamera m_EditorCamera;
		Ref<Textures2D> m_Texture;

		Ref<Scene> m_ActiveScene;
		Ref<Framebuffer> m_Framebuffer;

		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCameraEntity;
		
		bool m_PrimaryCamera = false;

		/* ImGui DockingSpace */
		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		/* Panels */
		SceneHierarchyPanel m_SceneHierarchy;

		/* Tile Grid */
		glm::uvec2 m_TileGridSize;
		glm::vec2 m_TileSize;
		glm::vec2 m_Origin;
	};

}