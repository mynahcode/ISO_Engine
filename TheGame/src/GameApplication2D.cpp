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
	m_SpriteSheet = IE::Textures2D::Create("assets/textures/RPGpack_sheet_2X.png");
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
	IE::Renderer2D::ResetStats();
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

		IE::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SpriteSheet, 1.0f, glm::vec4(1.0f));

		IE::Renderer2D::EndScene();
	}
}

void TheGame2D::OnImGuiRender()
{
	_IE_PROFILER_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = IE::Renderer2D::GetStats();

	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("DrawCalls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Text("Memory Usage: WIP"); // TODO: Add memory usage tracking

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void TheGame2D::OnEvent(IE::Event& ev)
{
	_IE_PROFILER_FUNCTION();
	m_CameraController.OnEvent(ev);
}
