#include "IsoEditorLayer.h"

#include <chrono>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace IE
{
    IsoEditorLayer::IsoEditorLayer()
        : Layer("IsoEngine Editor"), 
            m_CameraController((1920.0f / 1080.0f), 1)
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
        fbSpecs.Attachments = { 
            fbTextureFormats::RGBA8, 
            fbTextureFormats::RED_INTEGER, 
            fbTextureFormats::Depth
        };
        m_Framebuffer = Framebuffer::Create(fbSpecs);

        m_DefaultSpriteSheet = Textures2D::Create("assets/textures/spritesheet.png");

        ISOLOGGER_INFO("Creating reference object to Scene...\n");
        m_ActiveScene = CreateRef<Scene>();

        ISOLOGGER_INFO("Creating Editor Camera...\n");
        //m_EditorCamera = PerspectiveEditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

        ISOLOGGER_WARN("Creating Texture Manager object...\n");
        m_TextureManager = TextureManager(m_DefaultSpriteSheet, { 192.0f, 320.0f }, { 64.0f, 64.0f }, { 5, 3 });

        ISOLOGGER_WARN("Loading Default SpriteSheet...\n");
        m_ActiveScene->AddSpriteSheetTextures(m_TextureManager.GetTextureSprites());

        ISOLOGGER_WARN("Creating GridManager object...\n");
        m_Grids = GridManager(m_ActiveScene, { 4, 4 }, { 1.0f, -1.0f }, { 0,0 });

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_CameraEntity.AddComponent<CameraComponent>(fbSpecs.Width, fbSpecs.Height);

#if 0        
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

#endif

        ISOLOGGER_TRACE("Setting scene context for scene hierarchy...\n")
        m_SceneHierarchy.SetContext(m_ActiveScene);
    }

    void IsoEditorLayer::OnDetach()
    {
        _IE_PROFILER_FUNCTION();
    }

    void IsoEditorLayer::OnEvent(Event& ev)
    {
        _IE_PROFILER_FUNCTION();
        m_CameraController.OnEvent(ev);
        //m_EditorCamera.OnEvent(ev);

        EventDispatcher dispatcher(ev);

        dispatcher.Dispatch<KeyPressedEvent>(IE_BIND_EVENT_FN(IsoEditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(IE_BIND_EVENT_FN(IsoEditorLayer::OnMouseButtonPressed));
    }

    bool IsoEditorLayer::OnKeyPressed(KeyPressedEvent& ev)
    {
        // Keyboard Shortcuts
        if (ev.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        switch (ev.GetKeyCode())
        {
            case Key::N:
            {
                if (control)
                    NewScene();

                break;
            }
            case Key::O:
            {
                if (control)
                    LoadScene();

                break;
            }
            case Key::S:
            {
                if (control && shift)
                    SaveSceneAs();

                break;
            }
        }
    }

    bool IsoEditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& ev)
    {
        if (ev.GetMouseButton() == Mouse::ButtonLeft)
        {
            if (m_HoveredEntity)
            {
                if (m_ViewportHovered && !Input::IsKeyPressed(Key::LeftAlt))
                {
                    m_SceneHierarchy.SetSelectedEntity(m_HoveredEntity);
                }
            }
            else
            {
                if (m_ViewportHovered)
                {
                    ISOLOGGER_WARN("Resetting Selection Context\n");
                    if (m_SceneHierarchy.GetSelectionContext())
                    {
                        m_SceneHierarchy.ResetSelectionContext();
                    }
                }
            }
        }

        if (ev.GetMouseButton() == Mouse::ButtonRight)
        {
            if (m_HoveredEntity)
            {
                if (m_ViewportHovered)
                {
                    m_SceneHierarchy.SetSelectedEntity(m_HoveredEntity);
                }
            }
            else
            {
                if (m_ViewportHovered)
                {
                    ISOLOGGER_WARN("Resetting Selection Context\n");
                    if (m_SceneHierarchy.GetSelectionContext())
                    {
                        m_SceneHierarchy.ResetSelectionContext();
                    }
                }
            }
        }

        return false;
    }

    void IsoEditorLayer::OnUpdate(Timestep timestep)
    {
        _IE_PROFILER_FUNCTION();

        if (FramebufferSpecs spec = m_Framebuffer->GetFramebufferSpecs();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            //m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        //m_EditorCamera.OnUpdate(timestep);
        m_CameraController.OnUpdate(timestep);

        Renderer2D::ResetStats();

        RenderCommand::DepthTestingEnabled(false);
        m_Framebuffer->Bind();
        // Clearing entityID attachment to -1 to distinguish pixels (and entities) from background pixels.
        m_Framebuffer->ClearAttachment(1, -1);

        m_ActiveScene->OnUpdateEditor(timestep, m_CameraController);

        PollMousePosition();

        m_Framebuffer->UnBind();
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

                if (ImGui::MenuItem("Scene"))
                {
                    if (ImGui::MenuItem("New Scene", "ctrl+N"))
                    {
                        NewScene();
                    }

                    if (ImGui::MenuItem("Open Scene", "ctrl+O"))
                    {
                        LoadScene();
                    }

                    if (ImGui::MenuItem("Save Scene", "ctrl+S"))
                    {

                    }

                    if (ImGui::MenuItem("Save Scene As", "ctrl+shift+S"))
                    {
                        SaveSceneAs();
                    }
                }

                if (ImGui::MenuItem("Serialize"))
                {
                    SceneSerializer serializer(m_ActiveScene);
                    serializer.Serialize("assets/scenes/Test.isoe");
                }

                if (ImGui::MenuItem("Deserialize"))
                {
                    SceneSerializer serializer(m_ActiveScene);
                    serializer.Deserialize("assets/scenes/Test.isoe");
                }

                if (ImGui::MenuItem("Options"))
                {
                    ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                    ImGui::MenuItem("Padding", NULL, &opt_padding);
                }

                if (ImGui::MenuItem("Exit")) Application::Get().Close();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_SceneHierarchy.OnImGuiRender();
        
        //ImGui::ShowDemoWindow(); // IMGUI DEMO WINDOW *Keep for debug and learning purposes*

        ImGui::Begin("Statistics");

        std::string name = "None";

        if (m_HoveredEntity)
            name = m_HoveredEntity.GetComponent<TagComponent>().Tag;

        ImGui::Text("Hovered Entity: %s", name.c_str());

        auto stats = Renderer2D::GetStats();
        ImGui::Text("2D Renderer Stats:");
        ImGui::Text("DrawCalls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::Text("Texture Slots Bounded: %d / %d", stats.GetTextureSlotsInUse(), stats.GetMaxTextureSlots()); //TODO: fix maxTextureslots
        ImGui::Text("Memory Usage: WIP"); // TODO: Add memory usage tracking
        ImGui::End();

        /* ImGui Viewport Dockspace */
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");
        auto viewportOffset = ImGui::GetCursorPos(); // includes tab bar

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 vpPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { vpPanelSize.x, vpPanelSize.y };

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        auto windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
        m_ViewportBounds[0] = { minBound.x, minBound.y };
        m_ViewportBounds[1] = { maxBound.x, maxBound.y };

        /* Right-Clicking an entity in scene viewport */

        if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
        {
            auto selectedEntity = m_SceneHierarchy.GetSelectionContext();
            if (selectedEntity && selectedEntity.HasComponent<TileComponent>())
            {
                if (ImGui::MenuItem("Copy"))
                {
                    m_CopiedEntity = selectedEntity;
                }
                if (m_CopiedEntity && m_CopiedEntity != selectedEntity)
                {
                    if (ImGui::MenuItem("Paste"))
                    {
                        auto& src = m_CopiedEntity.GetComponent<SpriteRendererComponent>();
                        auto& dest = selectedEntity.GetComponent<SpriteRendererComponent>();
                        if (src.Texture && dest.Texture)
                        {
                            dest.Texture = src.Texture;
                        }
                        else if (!(src.SubTextures.empty() && dest.SubTextures.empty()))
                        {
                            dest.SubTextures = src.SubTextures;
                        }
                        
                        dest.Color = src.Color;
                    }
                }
                if (ImGui::MenuItem("Delete"))
                {

                }

            }
            ImGui::EndPopup();
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void IsoEditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchy.SetContext(m_ActiveScene);
    }

    void IsoEditorLayer::LoadScene()
    {
        std::optional<std::string> filepath = FileDialogs::OpenFile("IsoEngine Scene (*.isoe)\0*.isoe\0");
        if (filepath)
        {
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchy.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(*filepath);
        }
    }

    void IsoEditorLayer::SaveSceneAs()
    {
        std::optional<std::string> filepath = FileDialogs::SaveFile("IsoEngine Scene (*.isoe)\0*.isoe\0");
        if (filepath)
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(*filepath);
        }
    }

    void IsoEditorLayer::PollMousePosition()
    {
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        my = m_ViewportSize.y - my; // Accounting for ImGui flipping OpenGL coords.

        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_ViewportSize.x && mouseY < (int)m_ViewportSize.y)
        {
            int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            //ISOLOGGER_WARN("Pixel Data at position < {0}, {1} >: {2} \n", mouseX, mouseY, pixelData);
            m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }
    }

}