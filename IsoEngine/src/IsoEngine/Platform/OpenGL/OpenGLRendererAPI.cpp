#include "iepch.h"
#include "IsoEngine/Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace IE
{
	// TODO: Implement graphics API specific renderer profiler that can be enabled with ifdef define.
	void OpenGLRendererAPI::Init()
	{
		_IE_PROFILER_FUNCTION();

		ISOLOGGER_TRACE("OpenGLRendererAPI::Init() called...\n");
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);			// Enables GL depth testing 
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		ISOLOGGER_WARN("OpenGLRendererAPI::SetViewport() called...\n");
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);			// Ensure GL depth buffer being cleared too
	}

	void OpenGLRendererAPI::DepthTestingEnabled(bool toggle)
	{
		if (toggle)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(toggle);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			glDepthMask(toggle);
		}
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		//ISOLOGGER_TRACE("OpenGLRendererAPI::DrawIndexed() --> glDrawElements() w/ indexCount = {0}\n", count);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}
}