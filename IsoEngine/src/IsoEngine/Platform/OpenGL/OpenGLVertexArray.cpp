#include "iepch.h"
#include "IsoEngine/Platform/OpenGL/OpenGLVertexArray.h"
#include <glad/glad.h>

namespace IE
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		//IE_CORE_ASSERT(false, "Uknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_DEBUG("(GL) OpenGLVertexArray() Constructor Called...\n")
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_DEBUG("(GL) OpenGLVertexArray() Destructor Called...\n")
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_CRITICAL("(GL) Binding Vertex Arrays\n")
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_CRITICAL("(GL) Unbinding Vertex Arrays\n")
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_CRITICAL("Binding Vertex Buffers --> Size: {0}...\n", vertexBuffer->GetLayout().GetElements().size());
		/* Create OpenGL Context */
		Bind();

		IE_ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size() > 0, "Error: Vertex Buffer has no Layout!\n");


		/* Iterate through every element in layout */
		uint32_t vb_Index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			/* Layout of Buffer */
			glVertexAttribPointer(vb_Index + m_VertexBufferIndexOffset,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.IsNormalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			glEnableVertexAttribArray(vb_Index + m_VertexBufferIndexOffset);
			vb_Index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexBufferIndexOffset += static_cast<uint32_t>(layout.GetElements().size());
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_DEBUG("(GL) Binding VertexArray and IndexBuffer... \n")
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}