#pragma once

#include "IsoEngine/Renderer/Material.h"

namespace IE
{
	class OpenGLMaterial2D : public Material2D
	{
	public:
		OpenGLMaterial2D(const Ref<Shader>& shader, const Ref<Textures2D>& texture);
		OpenGLMaterial2D(const Ref<Shader>& shader, const Ref<SubTexture2D>& subtexture);
		OpenGLMaterial2D(const Ref<Shader>& shader, const Ref<Textures2D>& texture, const Ref<SubTexture2D>& subtexture);

		OpenGLMaterial2D(const OpenGLMaterial2D& other);
		virtual ~OpenGLMaterial2D();

		virtual uint32_t GetShaderRendererID() const override;
		virtual void SetShaderUniforms() override;

		Ref<Shader> GetShader() const { return m_Shader; }
		Ref<Textures2D> GetTexture() const { return m_Texture; }
		Ref<SubTexture2D> GetSubTexture() const { return m_SubTexture; }

		void SetShader(const Ref<Shader> shader); // To check for nullptr 
		void SetTexture(const Ref<Textures2D> texture) { m_Texture = texture; }
		void SetSubTexture(const Ref<SubTexture2D> subtexture) { m_SubTexture = subtexture; }

		bool operator==(const OpenGLMaterial2D& other) { return *this == other; }
	private:
		Ref<Shader> m_Shader;
		Ref<Textures2D> m_Texture;
		Ref<SubTexture2D> m_SubTexture;

		std::unordered_map<std::string, int> m_UniformInts;
		std::unordered_map<std::string, float> m_UniformFloats;
		std::unordered_map<std::string, glm::vec2> m_UniformVec2s;
		std::unordered_map<std::string, glm::vec3> m_UniformVec3s;
		std::unordered_map<std::string, glm::vec4> m_UniformVec4s;
		std::unordered_map<std::string, glm::mat4> m_UniformMat4s;
	};
}