#include "iepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "IsoEngine/Platform/OpenGL/OpenGLShader.h"

namespace IE
{
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: /* IE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); */
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(filepath);
		}

		/*IE_CORE_ASSERT(false, "Unknown Renderering API!"); */
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: /* IE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); */ 
				return nullptr;
			case RendererAPI::API::OpenGL: 
				return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		/*IE_CORE_ASSERT(false, "Unknown Renderering API!"); */
		return nullptr;
	}
}