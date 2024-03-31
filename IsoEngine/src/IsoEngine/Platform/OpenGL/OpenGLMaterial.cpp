#include "iepch.h"

#include "OpenGLMaterial.h"

#include <glad/glad.h>
namespace IE
{
	/*
	OpenGLMaterial2D::OpenGLMaterial2D(const Ref<Shader>& shader, const Ref<Textures2D>& texture)
		: m_Shader(shader), m_Texture(texture), m_SubTexture(nullptr)
	{

	}

	OpenGLMaterial2D::OpenGLMaterial2D(const Ref<Shader>& shader, const Ref<SubTexture2D>& subtexture)
		: m_Shader(shader), m_Texture(nullptr), m_SubTexture(subtexture)
	{

	}

	OpenGLMaterial2D::OpenGLMaterial2D(const Ref<Shader>& shader, const Ref<Textures2D>& texture, const Ref<SubTexture2D>& subtexture)
		: m_Shader(shader), m_Texture(texture), m_SubTexture(subtexture)
	{

	}
	*/

	OpenGLMaterial::OpenGLMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLint& diffuseTex, const GLint& specularTex)
		: m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_DiffuseTex(diffuseTex), m_SpecularTex(specularTex)
	{

	}

	OpenGLMaterial::OpenGLMaterial(const OpenGLMaterial& other)
	{
		m_Shader = other.m_Shader;
		m_Texture = other.m_Texture;
		m_SubTexture = other.m_SubTexture;

		m_UniformInts = other.m_UniformInts;
		m_UniformFloats = other.m_UniformFloats;
		m_UniformVec2s = other.m_UniformVec2s;
		m_UniformVec3s = other.m_UniformVec3s;
		m_UniformVec4s = other.m_UniformVec4s;
		m_UniformMat4s = other.m_UniformMat4s;
	}

	uint32_t OpenGLMaterial::GetShaderRendererID() const
	{
		return m_Shader->GetShaderID();
	}

	void OpenGLMaterial::SetShaderUniforms()
	{
		if (m_Shader)
		{
			m_Shader->SetFloat3("material.ambient", m_Ambient);
			m_Shader->SetFloat3("material.diffuse", m_Diffuse);
			m_Shader->SetFloat3("material.specular", m_Specular);
			for (const auto& iter : m_UniformInts)
			{
				m_Shader->SetInt(iter.first, iter.second);
			}

			for (const auto& iter : m_UniformFloats)
			{
				m_Shader->SetFloat(iter.first, iter.second);
			}

			for (const auto& iter : m_UniformVec2s)
			{
				m_Shader->SetFloat2(iter.first, iter.second);
			}

			for (const auto& iter : m_UniformVec3s)
			{
				m_Shader->SetFloat3(iter.first, iter.second);
			}

			for (const auto& iter : m_UniformVec4s)
			{
				m_Shader->SetFloat4(iter.first, iter.second);
			}

			for (const auto& iter : m_UniformMat4s)
			{
				m_Shader->SetMat4(iter.first, iter.second);
			}
		}
	}

	void OpenGLMaterial::SetShader(Ref<Shader> shader)
	{
		IE_ENGINE_ASSERT(shader, "Cannot set a nullptr shader!");
		if (shader)
			m_Shader = shader;
	}
}