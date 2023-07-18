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
		OrthographicCameraController m_CameraController;
		// Temporary 
		Ref<VertexArray> m_SquareVertexArray;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Textures2D> m_SpriteSheet;
		Ref<Textures2D> m_GrassTexture;
		Ref<SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;

		ShaderLibrary m_ShaderLibrary;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		struct ProfilerResult
		{
			const char* ProfilerName;
			float ProfilerTime;
		};

		std::vector<ProfilerResult> m_ProfilerResults;
	};

}