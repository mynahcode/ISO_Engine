#pragma once

#include "IsoEngine/IsoMacros.h"

#include "Window.h"
#include "IsoEngine/LayerStack.h"
#include "IsoEngine/Events/Event.h"
#include "IsoEngine/Events/ApplicationEvent.h"


namespace IE 
{

	class IsoEngine_API Application // export here, in game include.
	{
	public:
		Application();
		virtual ~Application(); // will be inherited by Game application

		void Run();

		void OnEvent(Event& ev);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& ev);

		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
	};

	// Defined in client
	Application* CreateApplication();
}
