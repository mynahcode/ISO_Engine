#pragma once

#include "IsoEngine/Renderer/Shader.h"

#include <glm/glm.hpp>

// TODO: remove
typedef unsigned int GLenum;

namespace IE
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& uMatrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& uMatrix);


	private:
		std::string ReadShaderFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string>PreProcessShaderSrc(const std::string& src);
		void CompileGLShader(const std::unordered_map<GLenum, std::string>& shaderSrc);
		uint32_t m_RendererID;
	};
}