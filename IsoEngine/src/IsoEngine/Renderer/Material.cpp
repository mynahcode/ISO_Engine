#include "iepch.h"

#include "Material.h"
#include "Renderer.h"

#include "IsoEngine/Platform/OpenGL/OpenGLMaterial.h"

#include <GLFW/glfw3.h>

namespace IE
{
	/*
	Ref<Material2D> Material2D::Create(const Ref<Shader>& shader, const Ref<Textures2D>& texture)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IE_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLMaterial2D>(shader, texture);
		}

		IE_ENGINE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}

	Ref<Material2D> Material2D::Create(const Ref<Shader>& shader, const Ref<SubTexture2D>& subtexture)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IE_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLMaterial2D>(shader, subtexture);
		}

		IE_ENGINE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}
	
	Ref<Material2D> Material2D::Create(const Ref<Shader>& shader, const Ref<Textures2D>& texture, Ref<SubTexture2D> subtexture)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IE_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLMaterial2D>(shader, texture, subtexture);
		}

		IE_ENGINE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}
	*/

	Ref<MaterialGL> MaterialGL::Create(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLint& diffuseTex, const GLint& specularTex)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IE_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLMaterial>(ambient, diffuse, specular, diffuseTex, specularTex);
		}

		IE_ENGINE_ASSERT(false, "Error: Unknown Renderer API");
		return nullptr;
	}
}