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
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& values) override;

		inline virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& uMatrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& uMatrix);


	private:
		std::string ReadShaderFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcessShaderSrc(const std::string& src);
		void CompileGLShader(const std::unordered_map<GLenum, std::string>& shaderSrc);
		uint32_t m_RendererID;
		std::string m_Name;
	};
}