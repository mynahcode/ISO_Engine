#include "iepch.h"
#include "IsoEngine/Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace IE
{

	static GLenum StringToShaderType(const std::string& shaderTypeString)
	{
		if (shaderTypeString == "vertex") return GL_VERTEX_SHADER;
		else if (shaderTypeString == "fragment" || shaderTypeString == "pixel") return GL_FRAGMENT_SHADER;
		//else IE_CORE_ASSSERT(false, "Unknown shader type!")
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) // input file stream: std::ifstream
	{
		std::string src = ReadShaderFile(filepath);
		auto shaderSrc = PreProcessShaderSrc(src);
		CompileGLShader(shaderSrc);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		CompileGLShader(sources);

	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // std::string -> const GLchar* <--> use c_str()
		glUniform1i(location, value);

	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // std::string -> const GLchar* <--> use c_str()
		glUniform1f(location, value);

	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // std::string -> const GLchar* <--> use c_str()
		glUniform2f(location, values.x, values.y);

	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // std::string -> const GLchar* <--> use c_str()
		glUniform3f(location, values.x, values.y, values.z);

	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // std::string -> const GLchar* <--> use c_str()
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& uMatrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // std::string -> const GLchar* <--> use c_str()
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(uMatrix));

	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& uMatrix)
	{
		// Retrieve the uniform matrix first then upload uniform mat4 to shader.
		GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // std::string -> const GLchar* <--> use c_str()
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uMatrix));
	}

	std::string OpenGLShader::ReadShaderFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			//IE_CORE_ERROR("Could not open Shader file %", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcessShaderSrc(const std::string& src)
	{
		std::unordered_map<GLenum, std::string> shaderSrc;

		const char* typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = src.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos);
			//IE_CORE_ASSERT(eol != std::string::npos, "Syntax error in preprocessing shader src");
			size_t begin = pos + typeTokenLen + 1; // TODO: Implement whitespace trimming
			std::string type = src.substr(begin, eol - begin);
			//IE_CORE_ASSERT(StringToShaderType(type), "Invalid shader type specified!");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(typeToken, nextLinePos);
			shaderSrc[StringToShaderType(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}

		return shaderSrc;
	}

	void OpenGLShader::CompileGLShader(const std::unordered_map<GLenum, std::string>& shaderSrc)
	{
		/* Adapted From https://www.khronos.org/opengl/wiki/Shader_Compilation */

		// Assuming vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shaderSrc.size());

		for (auto& keyval : shaderSrc)
		{
			GLenum shaderType = keyval.first;
			const std::string& source = keyval.second;

			// Create an empty shader handle
			GLuint shader = glCreateShader(shaderType);

			const GLchar* sourceString = source.c_str();
			glShaderSource(shader, 1, &sourceString, 0);

			// Compile the fragment shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				//IE_CORE_ERROR("%", infoLog.data());
				//IE_CORE_ASSERT(false, "Fragment Shader Compilation Failure!");

				break;
			}

			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		// Maybe a bad spot for this \/
		m_RendererID = program;
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			//IE_CORE_ERROR("%", infoLog.data());
			//IE_CORE_ASSERT(false, "Shader Link Compilation Failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDetachShader(program, id);
	}

}