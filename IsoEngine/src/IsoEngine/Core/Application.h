#pragma once

#include "IsoEngine/Core/IsoMacros.h"

#include "IsoEngine/Core/Window.h"
#include "IsoEngine/Core/LayerStack.h"
#include "IsoEngine/Events/Event.h"
#include "IsoEngine/Events/ApplicationEvent.h"

#include "IsoEngine/ImGui/ImGuiLayer.h"
#include "IsoEngine/Renderer/Shader.h"
#include "IsoEngine/Renderer/Buffer.h"
#include "IsoEngine/Renderer/VertexArray.h"

#include "IsoEngine/Renderer/OrthographicCamera.h"

#include "IsoEngine/Core/Timestep.h"

int main(int argc, char** arv);

namespace IE 
{

	class Application // export here, in game include.
	{
	public:
		Application(const std::string& name);
		virtual ~Application(); // will be inherited by Game application

		void Run();

		void OnEvent(Event& ev);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Close();

		static Application& Get() { return *s_Instance; }

		Window& GetWindow() { return *m_Window; }
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	private:
		bool OnWindowClose(WindowCloseEvent& ev);
		bool OnWindowResize(WindowResizeEvent& ev);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	// Defined in client
	Application* CreateApplication();
}
