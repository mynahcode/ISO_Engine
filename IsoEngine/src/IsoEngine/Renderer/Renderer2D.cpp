#include "iepch.h"
#include "IsoEngine/Renderer/Renderer2D.h"
#include "IsoEngine/Renderer/VertexArray.h"
#include "IsoEngine/Renderer/Shader.h"
#include "IsoEngine/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace IE
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoord;
		float TextureIndex;
		float TilingFactor;

		// Editor-Only
		int EntityID;
	};

	struct GridVertex
	{
		glm::vec3 Position;
	};

	struct Renderer2DStorage
	{
		static const uint32_t MAXQUADS = 10000;
		static const uint32_t MAXVERTICES = MAXQUADS * 4;
		static const uint32_t MAXINDICES = MAXQUADS * 6;
		static const uint32_t MAXTEXTURESLOTS = 32; // TODO: RendererCapabilities class.

		Ref<VertexArray> QuadVertexArray; // VAO
		Ref<VertexArray> GridVertexArray; // VAO
		Ref<VertexBuffer> QuadVertexBuffer; // VBO
		Ref<VertexBuffer> GridVertexBuffer; // VBO

		Ref<Shader> TextureShader;
		Ref<Textures2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		GridVertex* GridVertexBufferBase = nullptr;
		GridVertex* GridVertexBufferPtr = nullptr;

		std::array<Ref<Textures2D>, MAXTEXTURESLOTS> TextureSlots;
		uint32_t TextureSlotIndex = 1; // Slot 0 dedicated to white texture.

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Renderer2DStats Stats;


		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
	};


	static Renderer2DStorage s_Data2D;

	void Renderer2D::Init()
	{
		// TODO: Add reference counting system to ensure objects are only deleted AFTER all classes done using them.
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_DEBUG("Renderer2D::Init() called.\n ");

		s_Data2D.QuadVertexArray = VertexArray::Create();

		s_Data2D.QuadVertexBuffer = VertexBuffer::Create(s_Data2D.MAXVERTICES * sizeof(QuadVertex));
		s_Data2D.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float2, "a_TexCoord"},
			{ ShaderDataType::Float, "a_TexIndex"},
			{ ShaderDataType::Float, "a_TilingFactor"},
			{ ShaderDataType::Int, "a_EntityID"}
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

		s_Data2D.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data2D.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data2D.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data2D.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		_IE_PROFILER_FUNCTION();

		delete[] s_Data2D.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		_IE_PROFILER_FUNCTION();

		//ISOLOGGER_TRACE("Renderer2D::BeginScene() with a Camera Component... \n");

		s_Data2D.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		s_Data2D.TextureShader->SetMat4("u_ViewProjection", viewProjection); // API agnostic call, in OpenGL its a Uniform, in DX it is setconstantbuffer
		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		_IE_PROFILER_FUNCTION();

		//ISOLOGGER_TRACE("Renderer2D::BeginScene() with an Orthographic Camera... \n");
		s_Data2D.TextureShader->Bind();
		s_Data2D.TextureShader->SetMat4("u_ViewProjection", camera.GetVPMatrix()); // API agnostic call, in OpenGL its a Uniform, in DX it is setconstantbuffer
		
		StartBatch();
	}

	void Renderer2D::BeginScene(const PerspectiveEditorCamera& camera)
	{
		_IE_PROFILER_FUNCTION();

		//ISOLOGGER_TRACE("Renderer2D::BeginScene() with an IsoEngine Editor Camera... \n");
		s_Data2D.TextureShader->Bind();
		s_Data2D.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		StartBatch();
	}

	void Renderer2D::StartBatch()
	{
		s_Data2D.QuadIndexCount = 0;
		s_Data2D.QuadVertexBufferPtr = s_Data2D.QuadVertexBufferBase;

		s_Data2D.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		_IE_PROFILER_FUNCTION();

		// Logging Bounded Texture Slots
		s_Data2D.Stats.InUseTextureSlots = s_Data2D.TextureSlotIndex;
		Flush();
	}

	void Renderer2D::Draw(uint32_t attachmentID)
	{
		s_Data2D.TextureShader->Bind();
		GLint location = glGetUniformLocation(s_Data2D.TextureShader->GetShaderID(), "u_Textures");
		glBindSampler(attachmentID, location);
		RenderCommand::DrawIndexed(s_Data2D.QuadVertexArray, s_Data2D.QuadIndexCount);
		s_Data2D.Stats.DrawCalls++;
	}

	void Renderer2D::Flush()
	{
		if (s_Data2D.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data2D.QuadVertexBufferPtr - (uint8_t*)s_Data2D.QuadVertexBufferBase);
		s_Data2D.QuadVertexBuffer->SetData(s_Data2D.QuadVertexBufferBase, dataSize);

		// Bind Textures
		for (uint32_t i = 0; i < s_Data2D.TextureSlotIndex; i++)
			s_Data2D.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data2D.QuadVertexArray, s_Data2D.QuadIndexCount);
		s_Data2D.Stats.DrawCalls++;
	}

	void Renderer2D::NextBatch()
	{
		ISOLOGGER_INFO("Renderer2D::NextBatch() called.\n")

		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		_IE_PROFILER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		_IE_PROFILER_FUNCTION();

		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} }; // { Bottom-Left, Bottom-Right, Top-Right, Top-Left }

		if (s_Data2D.QuadIndexCount >= Renderer2DStorage::MAXINDICES) NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data2D.QuadVertexBufferPtr->Position = transform * s_Data2D.QuadVertexPositions[i];
			s_Data2D.QuadVertexBufferPtr->Color = color;
			s_Data2D.QuadVertexBufferPtr->TextureCoord = textureCoords[i];
			s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data2D.QuadVertexBufferPtr->EntityID = entityID;
			s_Data2D.QuadVertexBufferPtr++;
		}

		s_Data2D.QuadIndexCount += 6;
		s_Data2D.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		//constexpr float x = 2.0f, y = 3.0f;
		//constexpr float sheetWidth = 2560.0f, sheetHeight = 1664.0f;
		//constexpr float spriteWidth = 128.0f, spriteHeight = 128.0f;

		constexpr size_t quadVertexCount = 4;
		//constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		if (s_Data2D.QuadIndexCount >= Renderer2DStorage::MAXINDICES) 
			NextBatch();

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

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data2D.QuadVertexBufferPtr->Position = transform * s_Data2D.QuadVertexPositions[i];
			s_Data2D.QuadVertexBufferPtr->Color = tintColor;
			s_Data2D.QuadVertexBufferPtr->TextureCoord = textureCoords[i];
			s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data2D.QuadVertexBufferPtr->EntityID = entityID;
			s_Data2D.QuadVertexBufferPtr++;
		}

		s_Data2D.QuadIndexCount += 6;
		s_Data2D.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subTexture->GetTextureCoords();
		const Ref<Textures2D> texture = subTexture->GetTexture();

		if (s_Data2D.QuadIndexCount >= Renderer2DStorage::MAXINDICES) NextBatch();

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

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data2D.QuadVertexBufferPtr->Position = transform * s_Data2D.QuadVertexPositions[i];
			s_Data2D.QuadVertexBufferPtr->Color = color;
			s_Data2D.QuadVertexBufferPtr->TextureCoord = textureCoords[i];
			s_Data2D.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data2D.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data2D.QuadVertexBufferPtr->EntityID = entityID;
			s_Data2D.QuadVertexBufferPtr++;
		}

		s_Data2D.QuadIndexCount += 6;
		s_Data2D.Stats.QuadCount++;
	}

	// Rotation should be in radians.
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		_IE_PROFILER_FUNCTION();

		//transform = translation * rotation * scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		//transform = translation * rotation * scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		_IE_PROFILER_FUNCTION();

		//transform = translation * rotation * scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec2& size, SpriteRendererComponent& src, int entityID)
	{
		_IE_PROFILER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, src.Texture, 1.0f, src.Color, entityID);
	}

	void Renderer2D::DrawRotatedSprite(const glm::vec3& position, const glm::vec2& size, SpriteRendererComponent& src, int entityID, float rotation)
	{
		_IE_PROFILER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, src.Texture, 1.0f, src.Color, entityID);
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data2D.Stats, 0, sizeof(Renderer2DStats));
	}

	Renderer2D::Renderer2DStats Renderer2D::GetStats()
	{
		return s_Data2D.Stats;
	}
}