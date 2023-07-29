#include "iepch.h"
#include "IsoEngine/Renderer/RendererAPI.h"

#include "IsoEngine/Platform/OpenGL/OpenGLRendererAPI.h"

namespace IE
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    IE_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		IE_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}