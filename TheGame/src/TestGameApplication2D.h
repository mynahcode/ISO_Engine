#pragma once

#include <vector>

#include <IsoEngine.h>

class TestGame2D : public IE::Layer
{
public:
	TestGame2D();
	virtual ~TestGame2D() = default;

	/* IsoEngine Layer Class Functions */
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	/* IsoEngine Application Functions */
	void OnUpdate(IE::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(IE::Event& ev) override;

private:
	IE::OrthographicCameraController m_CameraController;
	// Temporary 
	IE::Ref<IE::VertexArray> m_SquareVertexArray;
	IE::Ref<IE::Shader> m_FlatColorShader;
	IE::Ref<IE::Framebuffer> m_Framebuffer;

	IE::Ref<IE::Textures2D> m_SpriteSheet;
	IE::Ref<IE::Textures2D> m_GrassTexture;
	IE::Ref<IE::SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	struct ProfilerResult
	{
		const char* ProfilerName;
		float ProfilerTime;
	};

	std::vector<ProfilerResult> m_ProfilerResults;
};