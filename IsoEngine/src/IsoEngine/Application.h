#pragma once

#include "IsoMacros.h"

#include "Window.h"
#include "IsoEngine/LayerStack.h"
#include "IsoEngine/Events/Event.h"
#include "IsoEngine/Events/ApplicationEvent.h"

#include "IsoEngine/ImGui/ImGuiLayer.h"
#include "IsoEngine/Renderer/Shader.h"
#include "IsoEngine/Renderer/Buffer.h"


namespace IE 
{

	class Application // export here, in game include.
	{
	public:
		Application();
		virtual ~Application() = default; // will be inherited by Game application

		void Run();

		void OnEvent(Event& ev);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& ev);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;

		/* Renderer IDs */
		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		static Application* s_Instance;
	};

	// Defined in client
	Application* CreateApplication();
}
