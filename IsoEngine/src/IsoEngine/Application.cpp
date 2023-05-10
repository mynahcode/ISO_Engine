#include "iepch.h"
#include "IsoEngine/Application.h"


namespace IE 
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	/* Application is implemented as a Singleton. */
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create()); // explicit constructor
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
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

	void Application::OnEvent(Event& ev)
	{
		ISOLOGGER_TRACE("Event Running");
		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	}

	bool Application::OnWindowClose(WindowCloseEvent& ev)
	{
		m_IsRunning = false;
		return true;
	}
}