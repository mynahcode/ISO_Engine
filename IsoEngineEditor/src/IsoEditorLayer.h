#pragma once

#include <vector>

#include <IsoEngine.h>

namespace IE
{

	class IsoEditorLayer : public IE::Layer
	{
	public:
		IsoEditorLayer();
		virtual ~IsoEditorLayer() = default;

		/* IsoEngine Layer Class Functions */
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		/* IsoEngine Application Functions */
		void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& ev) override;

	private:
		IsometricCameraController m_CameraController;

		Ref <Scene> m_ActiveScene;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Textures2D> m_SpriteSheet;
		Ref<Textures2D> m_GrassTexture;
		Ref<SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;

		Entity m_SquareEntity;
		Entity m_CameraEntity;

		/* ImGui DockingSpace */
		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};

}