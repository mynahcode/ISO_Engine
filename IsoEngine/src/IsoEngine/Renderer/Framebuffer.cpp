#include "iepch.h"
#include "Framebuffer.h"
#include "Renderer.h"

#include "IsoEngine/Platform/OpenGL/OpenGLFramebuffer.h"

namespace IE
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecs& specs)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			//IE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(specs);
		}

		//IE_ENGINE_ASSERT("Error: Unknown Renderer API", false);
		return nullptr;
	}
}