#include "iepch.h"

#include "IsoEngine/Renderer/VertexArray.h"
#include "IsoEngine/Renderer/Renderer.h"

#include "IsoEngine/Platform/OpenGL/OpenGLBuffer.h"
#include "IsoEngine/Platform/OpenGL/OpenGLVertexArray.h"

namespace IE
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				//IE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexArray>();
		}

		//IE_CORE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}
}