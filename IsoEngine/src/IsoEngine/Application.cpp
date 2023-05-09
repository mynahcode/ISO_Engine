#include "iepch.h"
#include "IsoEngine/Application.h"
#include "Events/ApplicationEvent.h"


namespace IE 
{
	/* Application is implemented as a Singleton. */
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create()); // explicit constructor
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			m_Window->OnUpdate();
		}
	}

}