#include "iepch.h"
#include "WindowsWindow.h"

namespace IE
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		ISOLOGGER_INFO("Creating Window '%s' with dimensions (%d,%d):", props.Title.c_str(), props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: Implement glfwTerminate on system shutdown
			int success = glfwInit();
			//if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW!");
			IE_ENGINE_ASSERT(success, "Could not initialize GLFW!")
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		if (m_Window == nullptr)
		{
			ISOLOGGER_FATAL("Failed to create GLFW Window in WindowsWindow.cpp!");
			std::cout << "Failed to create GLFW Window!" << std::endl;
			Shutdown();
		}
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}
