
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "IsoLogger/IsoLogger.h"

namespace IE 
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent ev(1280, 720);

		while (true);
	}

	Application* IE::CreateApplication() {
		return nullptr;
	}
}