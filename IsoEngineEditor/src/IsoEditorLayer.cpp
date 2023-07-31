#include "IsoEditorLayer.h"
#include <imgui/imgui.h>

#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace IE
{
    IsoEditorLayer::IsoEditorLayer()
        : Layer("IsoEngine Editor"), m_CameraController(1280.0f / 720.0f)
    {
        _IE_PROFILER_FUNCTION();
    }

    void IsoEditorLayer::OnAttach()
    {
        _IE_PROFILER_FUNCTION();

        //m_GrassTexture = IE::Textures2D::Create("assets/textures/grass.jpg");
        m_SpriteSheet = IE::Textures2D::Create("assets/textures/RPGpack_sheet_2X.png");

        //m_TextureStairs = IE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128, 128 });
        //m_TextureBarrel = IE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128, 128 });
        //m_TextureTree = IE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

        IE::FramebufferSpecs fbSpecs;
        fbSpecs.Width = 1280;
        fbSpecs.Height = 720;

        m_Framebuffer = IE::Framebuffer::Create(fbSpecs);

        //m_CameraController.SetZoomLevel(0.25f);
    }

    void IsoEditorLayer::OnDetach()
    {
        _IE_PROFILER_FUNCTION();
    }

    void IsoEditorLayer::OnUpdate(IE::Timestep timestep)
    {
        _IE_PROFILER_FUNCTION();

        if (IE::FramebufferSpecs spec = m_Framebuffer->GetFramebufferSpecs();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
        }

        // Update
        if (m_ViewportFocused)

        // Hook-in to the methods needed -- UPDATE
        m_CameraController.OnUpdate(timestep);

        /* RENDER */
        /* Starts scene and contains all information of scene.*/
        IE::Renderer2D::ResetStats();
        {
            _IE_PROFILER_SCOPE("Renderer Preparation Functions");
            m_Framebuffer->Bind();
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
            m_Framebuffer->UnBind();
        }
    }

    void IsoEditorLayer::OnImGuiRender()
    {
        _IE_PROFILER_FUNCTION();
        
        static bool dockingSpaceView = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", &dockingSpaceView, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
        // Disabling fullscreen would allow the window to be moved to the front of other windows,
        // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);

                if (ImGui::MenuItem("Exit")) IE::Application::Get().Close();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        /* Additions to the ImGui Dockspace for IsoEngine go below*/

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
        /********************************/
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");
        ImVec2 vpPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { vpPanelSize.x, vpPanelSize.y };

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        // ISOLOGGER_INFO("Viewport Panel Size: ( {%}, {%} )", vpPanelSize.x, vpPanelSize.y);
        ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1,0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
}

    void IsoEditorLayer::OnEvent(IE::Event& ev)
    {
        _IE_PROFILER_FUNCTION();
        m_CameraController.OnEvent(ev);
    }
}