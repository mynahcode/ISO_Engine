#include "iepch.h"

#include "IsoEngine/Renderer/GraphicsContext.h"
#include "IsoEngine/Renderer/Renderer.h"

#include "IsoEngine/Platform/OpenGL/OpenGLContext.h"

namespace IE {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    IE_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");  return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		IE_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}