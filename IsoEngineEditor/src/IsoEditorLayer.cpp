#include "IsoEditorLayer.h"

#include <chrono>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace IE
{
    IsoEditorLayer::IsoEditorLayer()
        : Layer("IsoEngine Editor"), m_CameraController(1920.0f / 1080.0f)
    {
        _IE_PROFILER_FUNCTION();
    }

    void IsoEditorLayer::OnAttach()
    {
        _IE_PROFILER_FUNCTION();

        ISOLOGGER_WARN("Creating Framebuffer object...\n");
        FramebufferSpecs fbSpecs;
        fbSpecs.Width = 1920;
        fbSpecs.Height = 1080;
        fbSpecs.Attachments = { fbTextureFormats::RGBA8, fbTextureFormats::Depth};
        m_Framebuffer = Framebuffer::Create(fbSpecs);

        ISOLOGGER_INFO("Creating reference object to Scene...\n");
        m_ActiveScene = CreateRef<Scene>();

        auto square = m_ActiveScene->CreateEntity("Square");
        square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
        m_SquareEntity = square;

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_CameraEntity.AddComponent<CameraComponent>(fbSpecs.Width, fbSpecs.Height);

        
        //m_SecondCameraEntity = m_ActiveScene->CreateEntity("Clip-Space Camera");
       //auto& cc2 = m_SecondCameraEntity.AddComponent<CameraComponent>();
        //cc2.isPrimary = false;

        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {
                //ISOLOGGER_TRACE("OnCreate() Called for Camera Controller \n");
            }

            void OnDestroy()
            {

            }

            void OnUpdate(Timestep ts)
            {
                auto& translation = GetComponent<TransformComponent>().Translation;
                float speed = 5.0f;

                if (Input::IsKeyPressed(Key::A))
                {
                    translation.x -= speed * ts;
                }

                if (Input::IsKeyPressed(Key::D))
                {
                    translation.x += speed * ts;
                }

                if (Input::IsKeyPressed(Key::W))
                {
                    translation.y += speed * ts;
                }

                if (Input::IsKeyPressed(Key::S))
                {
                    translation.y -= speed * ts;
                }
            }
        };
        
        //m_SecondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        ISOLOGGER_TRACE("Setting scene context for scene hierarchy...\n")
        m_SceneHierarchy.SetContext(m_ActiveScene);
    }

    void IsoEditorLayer::OnDetach()
    {
        _IE_PROFILER_FUNCTION();
    }

    void IsoEditorLayer::OnUpdate(Timestep timestep)
    {
        _IE_PROFILER_FUNCTION();

        if (FramebufferSpecs spec = m_Framebuffer->GetFramebufferSpecs();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        //m_Framebuffer->ClearAttachment(1, -1);
        m_ActiveScene->OnUpdate(timestep);
        m_Framebuffer->UnBind();
        RenderCommand::DepthTestingEnabled(true);
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

        ImGui::Begin("IsoEngine Dockspace", &dockingSpaceView, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("IsoEngine_DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {

                if (ImGui::MenuItem("Options"))
                {
                    ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                    ImGui::MenuItem("Padding", NULL, &opt_padding);
                }

                if (ImGui::MenuItem("Serialize"))
                {
                    //SceneSerializer serializer(m_ActiveScene);
                    //serializer.Serialize("Assets/scenes/Example.isoe");
                }

                if (ImGui::MenuItem("Deserialize"))
                {
                    //SceneSerializer serializer(m_ActiveScene);
                    //serializer.Deserialize("Assets/scenes/Example.isoe");
                }

                if (ImGui::MenuItem("Exit")) Application::Get().Close();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        /* Additions to the ImGui Dockspace for IsoEngine go below*/

        m_SceneHierarchy.OnImGuiRender();
        
        //ImGui::ShowDemoWindow(); // IMGUI DEMO WINDOW

        ImGui::Begin("Statistics");

        auto stats = Renderer2D::GetStats();

        ImGui::Text("2D Renderer Stats:");
        ImGui::Text("DrawCalls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::Text("Memory Usage: WIP"); // TODO: Add memory usage tracking
        ImGui::End();

        /********************************/
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 vpPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { vpPanelSize.x, vpPanelSize.y };

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
}

    void IsoEditorLayer::OnEvent(Event& ev)
    {
        _IE_PROFILER_FUNCTION();
        m_CameraController.OnEvent(ev);
    }
}