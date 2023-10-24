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
		else IE_ENGINE_ASSERT(false, "Unknown shader type!");
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) // input file stream: std::ifstream
	{
		_IE_PROFILER_FUNCTION();

		ISOLOGGER_TRACE("Creating OpenGLTexture shader from file: {0}... \n", filepath);
		std::string src = ReadShaderFile(filepath);
		auto shaderSrc = PreProcessShaderSrc(src);
		CompileGLShader(shaderSrc);

		std::filesystem::path path = filepath;
		m_Name = path.stem().string(); // Returns the file's name stripped of the extension.
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		_IE_PROFILER_FUNCTION();

		ISOLOGGER_TRACE("Creating Texture shader: < {0}, {1}, {2} > \n", name, vertexSrc, fragmentSrc);
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		CompileGLShader(sources);

	}

	OpenGLShader::~OpenGLShader()
	{
		_IE_PROFILER_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_DEBUG("(GL) Binding shader...\n")
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_DEBUG("(GL) Unbinding shader...\n")
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		_IE_PROFILER_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		_IE_PROFILER_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		_IE_PROFILER_FUNCTION();

		UploadUniformFloat2(name, values);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		_IE_PROFILER_FUNCTION();

		UploadUniformFloat3(name, values);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		_IE_PROFILER_FUNCTION(); 

		UploadUniformFloat4(name, values);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& values)
	{
		_IE_PROFILER_FUNCTION();
		//ISOLOGGER_TRACE("(OpenGL Shader) Setting mat4: {0} \n", name);
		UploadUniformMat4(name, values);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // std::string -> const GLchar* <--> use c_str()
		glUniform1i(location, value);

	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // std::string -> const GLchar* <--> use c_str()
		glUniform1iv(location, count, values);
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
		_IE_PROFILER_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
		}
		else
		{
			ISOLOGGER_CRITICAL("Could not open Shader file {}", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcessShaderSrc(const std::string& src)
	{
		_IE_PROFILER_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSrc;

		const char* typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = src.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos);
			IE_ENGINE_ASSERT(eol != std::string::npos, "Syntax error in preprocessing shader src");
			size_t begin = pos + typeTokenLen + 1; // TODO: Implement whitespace trimming
			std::string type = src.substr(begin, eol - begin);
			IE_ENGINE_ASSERT(StringToShaderType(type), "Invalid shader type specified!");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(typeToken, nextLinePos);
			shaderSrc[StringToShaderType(type)] = (pos == std::string::npos) ? src.substr(nextLinePos) : src.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSrc;
	}

	void OpenGLShader::CompileGLShader(const std::unordered_map<GLenum, std::string>& shaderSrc)
	{
		_IE_PROFILER_FUNCTION();

		/* Adapted From https://www.khronos.org/opengl/wiki/Shader_Compilation */

		// Assuming vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		// IE_CORE_ASSERT(shaderSrc.size() <= 2, "Error: only 2 shader sources");
		std::array<GLenum, 2> glShaderIDs; 

		int glShaderIDIndex = 0;
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

				ISOLOGGER_FATAL("(GL) Failed to compile shader: {0} \n", infoLog.data());
				IE_ENGINE_ASSERT(false, "Shader Compilation Failed! \n");
				break;
			}

			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}
		
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

			ISOLOGGER_FATAL("(GL) Failed to link shader to program: {}", infoLog.data());
			IE_ENGINE_ASSERT(false, "Shader Link Compilation Failure!");
			return;
		}
		ISOLOGGER_CRITICAL("OpenGLShader successfully linked...\n")
		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}
}