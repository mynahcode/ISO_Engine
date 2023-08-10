#include "iepch.h"

#include "IsoEngine/IsoLogger/IsoLogger.h"
#include "IsoEngine/Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace IE
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		//IE_CORE_ASSERT(windowHandle, "windowHandle is null!");
	}

	void OpenGLContext::Init()
	{
		_IE_PROFILER_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int glad_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		// IE_CORE_ASSERT(glad_status, "Failed to initialize Glad!");
		ISOLOGGER_INFO("***OPENGL INFO***\n");
		ISOLOGGER_INFO("Vendor: {0}\n", (char*)glGetString(GL_VENDOR));
		ISOLOGGER_INFO("Renderer: {0}\n", (char*)glGetString(GL_RENDERER));
		ISOLOGGER_INFO("Version: {0}\n", (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		_IE_PROFILER_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}
}