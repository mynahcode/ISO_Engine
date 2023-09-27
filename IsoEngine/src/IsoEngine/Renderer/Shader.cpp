#include "iepch.h"

#include "IsoEngine/Renderer/Shader.h"
#include "IsoEngine/Renderer/Renderer.h"

#include "IsoEngine/Platform/OpenGL/OpenGLShader.h"

namespace IE
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		ISOLOGGER_TRACE("Creating Texture shader from filepath: {0}... \n", filepath);
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: IE_ENGINE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(filepath);
		}

		IE_ENGINE_ASSERT(false, "Unknown Renderering API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		ISOLOGGER_TRACE("Creating Texture shader: < {0}, {1}, {2} > \n", name, vertexSrc, fragmentSrc);
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: IE_ENGINE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		IE_ENGINE_ASSERT(false, "Unknown Renderering API!"); 
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		IE_ENGINE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader Library failed to add shader!")
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		IE_ENGINE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader Library failed to add shader!")
		Add(name, shader);
	}	

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		IE_ENGINE_ASSERT(Exists(name), "Shader not found in Shader Library!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}