#pragma once

#include <IsoEngine.h>

class TheGame2D : public IE::Layer
{
public:
	TheGame2D();
	virtual ~TheGame2D() = default;

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

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};