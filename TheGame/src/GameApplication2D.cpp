#include "GameApplication2D.h"
#include <imgui/imgui.h>

#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename Fn>
class Timer
{
	// RAII Class, on construction it begins, on destructor call it ends.
	// Scope-based class
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			StopTimer();
	}

	void StopTimer()
	{
		auto m_StopTime = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
		long long stop = std::chrono::time_point_cast<std::chrono::microseconds>(m_StopTime).time_since_epoch().count();

		float duration = (stop - start) * 0.001f;

		m_Stopped = true;
		
		std::cout << "Client Function: "<< m_Name << " Duration: " << duration << "ms" << std::endl;

		m_Func({ m_Name, duration });
	}

private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
	bool m_Stopped;
};

// TODO: Fix define macro
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfilerResult profilerResult) { m_ProfilerResults.push_back(profilerResult); } )

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
	PROFILE_SCOPE("TheGame2D::OnUpdate()");

	{
		// Hook-in to the methods needed -- UPDATE
		PROFILE_SCOPE("TheGame2D::OrthographicCameraController::OnUpdate()");
		m_CameraController.OnUpdate(timestep);
	}

	/* RENDER */
	/* Starts scene and contains all information of scene.*/
	{
		PROFILE_SCOPE("Renderer Preparation Functions");
		IE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 }); // Shouldnt render onto clear color.
		IE::RenderCommand::Clear();
	}

	/* Renderer Flow */
	{
		PROFILE_SCOPE("Renderer Draw Functions");
		IE::Renderer2D::BeginScene(m_CameraController.GetCamera()); // parameters should be: lights, environment

		IE::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f }); // Testing translation of quad rendering
		IE::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f }); // Non-Uniform Quad render test
		IE::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_GrassTexture); // Blending Test for Textures -- z coordinate -0.1 pushes quad behind other textures

		IE::Renderer2D::EndScene();
	}
}

void TheGame2D::OnImGuiRender()
{
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
	m_CameraController.OnEvent(ev);
}