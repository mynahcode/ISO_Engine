#pragma once

#include "IsoEngine/IsoMacros.h"
#include "Events/Event.h"
#include "Window.h"

namespace IE 
{

	class IsoEngine_API Application // export here, in game include.
	{
	public:
		Application();
		virtual ~Application(); // will be inherited by Game application

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;
	};

	// Defined in client
	Application* CreateApplication();
}
