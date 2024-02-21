#include "iepch.h"

#include "OpenGLMaterial.h"

namespace IE
{
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

	OpenGLMaterial2D::OpenGLMaterial2D(const OpenGLMaterial2D& other)
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

	uint32_t OpenGLMaterial2D::GetShaderRendererID() const
	{
		return m_Shader->GetShaderID();
	}

	void OpenGLMaterial2D::SetShaderUniforms()
	{
		if (m_Shader)
		{
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

	void OpenGLMaterial2D::SetShader(Ref<Shader> shader)
	{
		IE_ENGINE_ASSERT(shader, "Cannot set a nullptr shader!");
		if (shader)
			m_Shader = shader;
	}
}