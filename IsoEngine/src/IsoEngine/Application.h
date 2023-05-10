#pragma once

#include "IsoEngine/IsoMacros.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"


namespace IE 
{

	class IsoEngine_API Application // export here, in game include.
	{
	public:
		Application();
		virtual ~Application(); // will be inherited by Game application

		void Run();

		void OnEvent(Event& ev);

	private:
		bool OnWindowClose(WindowCloseEvent& ev);

		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;
	};

	// Defined in client
	Application* CreateApplication();
}
