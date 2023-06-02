#include "iepch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "IsoEngine/Platform/OpenGL/OpenGLBuffer.h"

namespace IE
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
					//IE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!");
					return nullptr;

			case RendererAPI::API::OpenGL:
					return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		//IE_CORE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				//IE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		//IE_CORE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}
}