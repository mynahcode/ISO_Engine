#include "iepch.h"
#include "Textures.h"
#include "Renderer.h"

#include "IsoEngine/Platform/OpenGL/OpenGLTextures.h"

namespace IE
{
	Ref<Textures2D> Textures2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: /* IE_CORE_ASSERT(false, "RendererAPI::None isn't supported! */ return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTextures2D>(width, height);
		}

		/* IE_CORE_ASSERT(false, "Unknown RendererAPI! */
		return nullptr;
	}

	Ref<Textures2D> Textures2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: /* IE_CORE_ASSERT(false, "RendererAPI::None isn't supported! */ return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTextures2D>(path);
		}

		/* IE_CORE_ASSERT(false, "Unknown RendererAPI! */
		return nullptr;
	}
}