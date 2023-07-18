#include "TestGameApplication2D.h"
#include <imgui/imgui.h>

#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


TestGame2D::TestGame2D()
    : Layer("TestGame2D"), m_CameraController(1280.0f / 720.0f)
{
    _IE_PROFILER_FUNCTION();
}

void TestGame2D::OnAttach()
{
    _IE_PROFILER_FUNCTION();
    m_SpriteSheet = IE::Textures2D::Create("assets/textures/RPGpack_sheet_2X.png");

    //m_TextureStairs = IE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128, 128 });
    //m_TextureBarrel = IE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128, 128 });
    //m_TextureTree = IE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });
}

void TestGame2D::OnDetach()
{
    _IE_PROFILER_FUNCTION();
}

void TestGame2D::OnUpdate(IE::Timestep timestep)
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

        //IE::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_TextureStairs, 1.0f, glm::vec4(1.0f));
        //IE::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_TextureBarrel, 1.0f, glm::vec4(1.0f));
        //IE::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.5f }, { 1.0f, 2.0f }, m_TextureTree, 1.0f, glm::vec4(1.0f));
        IE::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_SpriteSheet, 1.0f, glm::vec4(1.0f));

        IE::Renderer2D::EndScene();
    }
}

void TestGame2D::OnImGuiRender()
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

        uint32_t textureID = m_SpriteSheet->GetRendererID();
        ImGui::Image((void*)textureID, ImVec2(1280.0f, 720.0f), ImVec2{ 0, 1 }, ImVec2{ 1,0 });
        ImGui::End();
}

void TestGame2D::OnEvent(IE::Event& ev)
{
    _IE_PROFILER_FUNCTION();
    m_CameraController.OnEvent(ev);
}