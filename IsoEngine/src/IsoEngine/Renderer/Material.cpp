#include "iepch.h"
#include "Material.h"
#include "Renderer.h"

namespace IE
{
	Ref<TileMaterial> TileMaterial::Create(uint32_t width, uint32_t height, Ref<Textures2D> tileTexture, Ref<Shader> shader)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IE_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return nullptr;
			//return CreateRef<OpenGLTileMaterial>(width, height, tileTexture, shader);
		}

		IE_ENGINE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}

	Ref<TileMaterial> TileMaterial::Create(uint32_t width, uint32_t height, Ref<SubTexture2D> tileTexture, Ref<Shader> shader)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IE_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return nullptr;
			//return CreateRef<OpenGLTileMaterial>(width, height, tileTexture, shader);
		}

		IE_ENGINE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}
	
}