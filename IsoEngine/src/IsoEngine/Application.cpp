#include "iepch.h"
#include "IsoEngine/Application.h"
#include "IsoEngine/IsoLogger/IsoLogger.h"

#include <gl/GL.h>
#include <gl/GLU.h>

namespace IE 
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	/* Application is implemented as a Singleton. */

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this; // Singleton gets set when we construct the IsoEngine Application and should only be one.

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

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& ev)
	{

		ISOLOGGER_INFO("%", ev);
		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto iter = m_LayerStack.end(); iter != m_LayerStack.begin(); )
		{
			(*--iter)->OnEvent(ev);
			if (ev.m_Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& ev)
	{
		m_IsRunning = false;
		return true;
	}
}