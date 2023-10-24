#include "iepch.h"
#include "IsoEngine/Platform/Windows/WindowsWindow.h"
#include "IsoEngine/IsoLogger/IsoLogger.h"
#include "IsoEngine/Events/MouseEvent.h"
#include "IsoEngine/Events/KeyEvent.h"
#include "IsoEngine/Events/ApplicationEvent.h"
#include "IsoEngine/Platform/OpenGL/OpenGLContext.h"

namespace IE
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* desc)
	{
		// TODO: Fix ISOLOGGER error logs AFTER refactoring IsoLogger.h
		// ISOLOGGER_CRITICAL("GLFW Callback Error (Code: %)", error);
	}

	Scope<Window> Window::Create(const WindowProps& props)
	{
		_IE_PROFILER_FUNCTION();

		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		_IE_PROFILER_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		_IE_PROFILER_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		_IE_PROFILER_FUNCTION();
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		ISOLOGGER_TRACE("Creating Window << {0} >> with dimensions ({1}, {2})\n", props.Title.c_str(), props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			_IE_PROFILER_SCOPE("WindowsWindow::Init()->glfwInit()");
			ISOLOGGER_TRACE("Initializing GLFW...\n");
			int success = glfwInit();
			IE_ENGINE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		
		{
			_IE_PROFILER_SCOPE("WindowsWindow::Init()->glfwCreateWindow()");
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}

		if (m_Window == nullptr)
		{
			IELogger::IsoLogger::SetPriority(IELogger::IELogger_Priority::FATAL);
			ISOLOGGER_FATAL("Failed to create GLFW Window in WindowsWindow.cpp!\n");
			Shutdown();
		}
		++s_GLFWWindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();


		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set the GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, false);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.EventCallback(event);
					break;
				}
				}
			});

		// Unicode characters
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		_IE_PROFILER_FUNCTION();

		glfwDestroyWindow(m_Window);

		if (--s_GLFWWindowCount == 0)
		{
			IELogger::IsoLogger::SetPriority(IELogger::IELogger_Priority::CRITICAL);
			ISOLOGGER_TRACE("Terminating GLFW in WindowsWindow::Shutdown()\n");
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_CRITICAL("Windowswindow swapbuffers...\n");
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		_IE_PROFILER_FUNCTION();
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
