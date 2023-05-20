#include "iepch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "IsoEngine/Platform/OpenGL/OpenGLBuffer.h"
#include "IsoEngine/Platform/OpenGL/OpenGLVertexArray.h"

namespace IE
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				//IE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();
		}

		//IE_CORE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}
}