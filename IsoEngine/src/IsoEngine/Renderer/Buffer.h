#pragma once

#include "IsoEngine/Core/IsoMacros.h"

namespace IE
{

	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	// Converting function between ShaderDataType to it's actual size.
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		// Default case of None should not occur!
		switch (type)
		{
			case ShaderDataType::Float:		return 4;					// Standard float size
			case ShaderDataType::Float2:	return 4 * 2;				// Vec2 -- vec of 2 floats
			case ShaderDataType::Float3:	return 4 * 3;				// Vec3 -- vec of 3 floats
			case ShaderDataType::Float4:	return 4 * 4;				// Vec4 -- vec of 4 floats
			case ShaderDataType::Mat3:		return 4 * 3 * 3;			// 3x3 Matrix
			case ShaderDataType::Mat4:		return 4 * 4 * 4;			// 4x4 Matrix
			case ShaderDataType::Int:		return 4;					// Standard int size
			case ShaderDataType::Int2:		return 4 * 2;				// Vec2 -- vec of 2 ints
			case ShaderDataType::Int3:		return 4 * 3;				// Vec3 -- vec of 3 ints
			case ShaderDataType::Int4:		return 4 * 4;				// Vec4 -- vec of 4 ints
			case ShaderDataType::Bool:		return 1;
		}

		//IE_CORE_ASSERT(false, "Error: Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		size_t Offset;
		uint32_t Size;
		bool IsNormalized;

		BufferElement() = default;

		// BufferElements Constructor with Initializer List
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), IsNormalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			//IE_CORE_ASSERT(false, "Error: Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:

		BufferLayout() {}

		// Constructor with const reference to InitializerList for BufferElement
		BufferLayout(const std::initializer_list<BufferElement>& elements) 
			: m_Elements(elements)
		{
			CalcOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		// Iterators
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		// Const Iterator
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	
	private:
		void CalcOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void  UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};
}