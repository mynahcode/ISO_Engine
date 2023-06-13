#include "GameApplication2D.h"
#include <imgui/imgui.h>

#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


TheGame2D::TheGame2D()
	: Layer("TheGame2D"), m_CameraController(1280.0f / 720.0f)
{
	_IE_PROFILER_FUNCTION();
}

void TheGame2D::OnAttach()
{
	_IE_PROFILER_FUNCTION();

	m_GrassTexture = IE::Textures2D::Create("assets/textures/grass.jpg");
}

void TheGame2D::OnDetach()
{
	_IE_PROFILER_FUNCTION();
}

void TheGame2D::OnUpdate(IE::Timestep timestep)
{
	_IE_PROFILER_FUNCTION();

	// Hook-in to the methods needed -- UPDATE
	m_CameraController.OnUpdate(timestep);

	/* RENDER */
	/* Starts scene and contains all information of scene.*/
	{
		_IE_PROFILER_SCOPE("Renderer Preparation Functions");
		IE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 }); // Shouldnt render onto clear color.
		IE::RenderCommand::Clear();
	}

	/* Renderer Flow */
	{
		static float rotation = 0.0f;
		rotation += timestep * 15.0f;

		_IE_PROFILER_SCOPE("Renderer Draw Functions");
		IE::Renderer2D::BeginScene(m_CameraController.GetCamera()); // parameters should be: lights, environment

		IE::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, rotation, { 0.8f, 0.2f, 0.3f, 1.0f }); // Testing translation of quad rendering
		IE::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f }); // Non-Uniform Quad render test
		IE::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, rotation, m_GrassTexture, 5.0f, glm::vec4(1.0f)); // Blending Test for Textures -- z coordinate -0.1 pushes quad behind other textures

		IE::Renderer2D::EndScene();
	}
}

void TheGame2D::OnImGuiRender()
{
	_IE_PROFILER_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for (auto& result : m_ProfilerResults)
	{
		char profileBuffer[100];
		strcpy(profileBuffer, " %.3fms ");
		strcat(profileBuffer, result.ProfilerName);
		ImGui::Text(profileBuffer, result.ProfilerTime);
	}
	m_ProfilerResults.clear();

	ImGui::End();
}

void TheGame2D::OnEvent(IE::Event& ev)
{
	_IE_PROFILER_FUNCTION();
	m_CameraController.OnEvent(ev);
}