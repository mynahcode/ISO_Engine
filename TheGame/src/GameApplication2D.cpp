#include "GameApplication2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TheGame2D::TheGame2D()
	: Layer("TheGame2D"), m_CameraController(1280.0f / 720.0f)
{
}

void TheGame2D::OnAttach()
{
	m_GrassTexture = IE::Textures2D::Create("assets/textures/grass.jpg");
}

void TheGame2D::OnDetach()
{

}

void TheGame2D::OnUpdate(IE::Timestep timestep)
{
	// Hook-in to the methods needed -- UPDATE
	m_CameraController.OnUpdate(timestep);

	/* RENDER */
	/* Starts scene and contains all information of scene.*/
	IE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 }); // Shouldnt render onto clear color.
	IE::RenderCommand::Clear();

	/* Renderer Flow */
	IE::Renderer2D::BeginScene(m_CameraController.GetCamera()); // parameters should be: lights, environment

	IE::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f }); // Testing translation of quad rendering
	IE::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f }); // Non-Uniform Quad render test
	IE::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_GrassTexture); // Blending Test for Textures -- z coordinate -0.1 pushes quad behind other textures

	IE::Renderer2D::EndScene();
}

void TheGame2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void TheGame2D::OnEvent(IE::Event& ev)
{
	m_CameraController.OnEvent(ev);
}