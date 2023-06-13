#include "iepch.h"
#include "IsoEngine/Renderer/Renderer2D.h"

#include "IsoEngine/Renderer/VertexArray.h"
#include "IsoEngine/Renderer/Shader.h"
#include "IsoEngine/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IE
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoord;
		float TextureIndex;
		float TilingFactor;
	};

	struct Renderer2DStorage
	{
		const uint32_t MAXQUADS = 10000;
		const uint32_t MAXVERTICES = MAXQUADS * 4;
		const uint32_t MAXINDICES = MAXQUADS * 6;
		static const uint32_t MAXTEXTURESLOTS = 32; // TODO: RendererCapabilities class.

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Textures2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Textures2D>, MAXTEXTURESLOTS> TextureSlots;
		uint32_t TextureSlotIndex = 1; // Slot 0 dedicated to white texture.
	};

	static Renderer2DStorage s_Data2D;

	void Renderer2D::Init()
	{
		// TODO: Add reference counting system to ensure objects are only deleted AFTER all classes done using them.
		_IE_PROFILER_FUNCTION();

		s_Data2D.QuadVertexArray = VertexArray::Create();

		s_Data2D.QuadVertexBuffer = VertexBuffer::Create(s_Data2D.MAXVERTICES * sizeof(QuadVertex));
		s_Data2D.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float2, "a_TextureCoord"},
			{ ShaderDataType::Float, "a_TextureIndex"},
			{ ShaderDataType::Float, "a_TilingFactor"}
			});
		s_Data2D.QuadVertexArray->AddVertexBuffer(s_Data2D.QuadVertexBuffer);

		s_Data2D.QuadVertexBufferBase = new QuadVertex[s_Data2D.MAXVERTICES];

		uint32_t* quadIndices = new uint32_t[s_Data2D.MAXINDICES];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data2D.MAXINDICES; i += 6)
		{
			quadIndices[i] = offset;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> m_quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data2D.MAXINDICES);
		s_Data2D.QuadVertexArray->SetIndexBuffer(m_quadIndexBuffer);

		delete[] quadIndices;

		s_Data2D.WhiteTexture = Textures2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data2D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t)); // 4 bytes

		int32_t samplers[s_Data2D.MAXTEXTURESLOTS];
		for (uint32_t i = 0; i < s_Data2D.MAXTEXTURESLOTS; i++)
			samplers[i] = i;

		s_Data2D.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data2D.TextureShader->Bind();
		s_Data2D.TextureShader->SetIntArray("u_Textures", samplers, s_Data2D.MAXTEXTURESLOTS);				// Texture slot that sampler samples from is slot 0.

		s_Data2D.TextureSlots[0] = s_Data2D.WhiteTexture;

	}

	void Renderer2D::Shutdown()
	{
		_IE_PROFILER_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		_IE_PROFILER_FUNCTION();

		s_Data2D.TextureShader->Bind();
		s_Data2D.TextureShader->SetMat4("u_ViewProjection", camera.GetVPMatrix()); // API agnostic call, in OpenGL its a Uniform, in DX it is setconstantbuffer

		s_Data2D.QuadIndexCount = 0;
		s_Data2D.QuadVertexBufferPtr = s_Data2D.QuadVertexBufferBase;

		s_Data2D.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		_IE_PROFILER_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data2D.QuadVertexBufferPtr - (uint8_t*)s_Data2D.QuadVertexBufferBase;
		s_Data2D.QuadVertexBuffer->SetData(s_Data2D.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		// Bind Textures
		for (uint32_t i = 0; i < s_Data2D.TextureSlotIndex; i++)
		{
			s_Data2D.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data2D.QuadVertexArray, s_Data2D.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		_IE_PROFILER_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		_IE_PROFILER_FUNCTION();

		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		s_Data2D.QuadVertexBufferPtr->Position = position; // Bottom-Left
		s_Data2D.QuadVertexBufferPtr->Color = color;
		s_Data2D.QuadVertexBufferPtr->TextureCoord = { 0.0f, 0.0f };
		s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data2D.QuadVertexBufferPtr++;		
		
		s_Data2D.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f }; // Bottom-Right
		s_Data2D.QuadVertexBufferPtr->Color = color;
		s_Data2D.QuadVertexBufferPtr->TextureCoord = { 1.0f, 0.0f };
		s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data2D.QuadVertexBufferPtr++;		
		
		s_Data2D.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f }; // Top-Right
		s_Data2D.QuadVertexBufferPtr->Color = color;
		s_Data2D.QuadVertexBufferPtr->TextureCoord = { 1.0f, 1.0f };
		s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data2D.QuadVertexBufferPtr++;		
		
		s_Data2D.QuadVertexBufferPtr->Position = { position.x, position.y+ size.y, 0.0f }; // Top-Left
		s_Data2D.QuadVertexBufferPtr->Color = color;
		s_Data2D.QuadVertexBufferPtr->TextureCoord = { 0.0f, 1.0f };
		s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data2D.QuadVertexBufferPtr++;

		s_Data2D.QuadIndexCount += 6;
	
		//s_Data2D.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		//s_Data2D.WhiteTexture->Bind();

		/* Calculate the transform matrix */
		// TODO: Add rotation matrix to the calculation for transform.
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); // transform matrix = translation matrix * rotation matrix * scale matrix (TRS), scaling done along x and y axes not z.
		//s_Data2D.TextureShader->SetMat4("u_Transform", transform); // Must be set on a per-draw basis

		/* Draw Call */
		//s_Data2D.QuadVertexArray->Bind();
		//RenderCommand::DrawIndexed(s_Data2D.QuadVertexArray);
		
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data2D.TextureSlotIndex; i++)
		{
			if (*s_Data2D.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data2D.TextureSlotIndex;
			s_Data2D.TextureSlots[s_Data2D.TextureSlotIndex] = texture;
			s_Data2D.TextureSlotIndex++;
		}

		s_Data2D.QuadVertexBufferPtr->Position = position; // Bottom-Left
		s_Data2D.QuadVertexBufferPtr->Color = color;
		s_Data2D.QuadVertexBufferPtr->TextureCoord = { 0.0f, 0.0f };
		s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data2D.QuadVertexBufferPtr++;

		s_Data2D.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f }; // Bottom-Right
		s_Data2D.QuadVertexBufferPtr->Color = color;
		s_Data2D.QuadVertexBufferPtr->TextureCoord = { 1.0f, 0.0f };
		s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data2D.QuadVertexBufferPtr++;

		s_Data2D.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f }; // Top-Right
		s_Data2D.QuadVertexBufferPtr->Color = color;
		s_Data2D.QuadVertexBufferPtr->TextureCoord = { 1.0f, 1.0f };
		s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data2D.QuadVertexBufferPtr++;

		s_Data2D.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f }; // Top-Left
		s_Data2D.QuadVertexBufferPtr->Color = color;
		s_Data2D.QuadVertexBufferPtr->TextureCoord = { 0.0f, 1.0f };
		s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data2D.QuadVertexBufferPtr++;

		s_Data2D.QuadIndexCount += 6;

		//s_Data2D.TextureShader->SetFloat4("u_Color", tintColor);
		//s_Data2D.TextureShader->SetFloat("u_TilingFactor", tilingFactor); 
		//texture->Bind();

		/* Calculate the transform matrix */
		// TODO: Add rotation matrix to the calculation for transform.
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); // transform matrix = translation matrix * rotation matrix * scale matrix (TRS), scaling done along x and y axes not z.
		//s_Data2D.TextureShader->SetMat4("u_Transform", transform); // Must be set on a per-draw basis

		/* Draw Call */
		//s_Data2D.QuadVertexArray->Bind();
		//RenderCommand::DrawIndexed(s_Data2D.QuadVertexArray);
	}

	// Rotation should be in radians.
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		_IE_PROFILER_FUNCTION();

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		_IE_PROFILER_FUNCTION();

		s_Data2D.TextureShader->SetFloat4("u_Color", color);
		s_Data2D.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data2D.WhiteTexture->Bind();

		/* Calculate the transform matrix */
		// TODO: Add rotation matrix to the calculation for transform.
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); // transform matrix = translation matrix * rotation matrix * scale matrix (TRS), scaling done along x and y axes not z.
		s_Data2D.TextureShader->SetMat4("u_Transform", transform); // Must be set on a per-draw basis

		/* Draw Call */
		s_Data2D.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data2D.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		s_Data2D.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data2D.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		/* Calculate the transform matrix */
		// TODO: Add rotation matrix to the calculation for transform.
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); // transform matrix = translation matrix * rotation matrix * scale matrix (TRS), scaling done along x and y axes not z.
		s_Data2D.TextureShader->SetMat4("u_Transform", transform); // Must be set on a per-draw basis

		/* Draw Call */
		s_Data2D.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data2D.QuadVertexArray);
	}
}