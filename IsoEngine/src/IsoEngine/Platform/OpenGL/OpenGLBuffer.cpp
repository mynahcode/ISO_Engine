#include "iepch.h"
#include "IsoEngine/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace IE
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		_IE_PROFILER_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	/* Vertex Buffer Implementation for OpenGL */
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		_IE_PROFILER_FUNCTION();

		/* Upload data to GPU from CPU */
		glCreateBuffers(1, &m_RendererID);	
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		_IE_PROFILER_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		_IE_PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		_IE_PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/* Index Buffer Implementation for OpenGL */
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		_IE_PROFILER_FUNCTION();

		/* Upload data to GPU from CPU */
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		_IE_PROFILER_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		_IE_PROFILER_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		_IE_PROFILER_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}