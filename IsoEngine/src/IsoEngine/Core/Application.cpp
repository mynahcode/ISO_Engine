#include "iepch.h"
#include "IsoEngine/Core/IsoMacros.h"
#include "IsoEngine/Core/Application.h"

#include "IsoEngine/Renderer/Renderer.h"

#include "IsoEngine/Core/Input.h"
#include "IsoEngine/IsoLogger/IsoLogger.h"

#include <GLFW/glfw3.h>

namespace IE 
{
	/* Application is implemented as a Singleton. */

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		/* Create GLFW Window */
		//IE_CORE_ASSERT(!s_Instance, "Application already created!");
		s_Instance = this; // Singleton gets set when we construct the IsoEngine Application and should only be one.

		m_Window = std::unique_ptr<Window>(Window::Create());		// explicit constructor
		m_Window->SetEventCallback(IE_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			/* imgui debug layer */
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

			//auto [x, y] = Input::GetMousePos(); // DEBUG
			//ISOLOGGER_INFO("<%, %>", x, y);
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

		//ISOLOGGER_INFO("%", ev);
		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<WindowCloseEvent>(IE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(IE_BIND_EVENT_FN(Application::OnWindowResize));

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


	bool Application::OnWindowResize(WindowResizeEvent& ev)
	{
		if (ev.GetWidth() == 0 || ev.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(ev.GetWidth(), ev.GetHeight());
		IELogger::IsoLogger::SetPriority(IELogger::IELogger_Priority::WARN);
		ISOLOGGER_WARN("Window Resized: (%, %)", ev.GetWidth(), ev.GetWidth());
		return true;
	}
}