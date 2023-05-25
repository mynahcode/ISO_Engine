#include "iepch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace IE
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case IE::ShaderDataType::Float:		return GL_FLOAT;
			case IE::ShaderDataType::Float2:	return GL_FLOAT;
			case IE::ShaderDataType::Float3:	return GL_FLOAT;
			case IE::ShaderDataType::Float4:	return GL_FLOAT;
			case IE::ShaderDataType::Mat3:		return GL_FLOAT;
			case IE::ShaderDataType::Mat4:		return GL_FLOAT;
			case IE::ShaderDataType::Int:		return GL_INT;
			case IE::ShaderDataType::Int2:		return GL_INT;
			case IE::ShaderDataType::Int3:		return GL_INT;
			case IE::ShaderDataType::Int4:		return GL_INT;
			case IE::ShaderDataType::Bool:		return GL_BOOL;
		}

		//IE_CORE_ASSERT(false, "Uknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		/* Create OpenGL Context */
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		//IE_CORE_ASSET(vertexBuffer->GetLayout().GetElements().size(), "Error: Vertex Buffer has no Layout!")

		/* Iterate through every element in layout */
		uint32_t vb_Index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			/* Layout of Buffer */
			glEnableVertexAttribArray(vb_Index + m_VertexBufferIndexOffset);
			glVertexAttribPointer(vb_Index + m_VertexBufferIndexOffset,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.IsNormalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset);

			vb_Index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexBufferIndexOffset += layout.GetElements().size();
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}