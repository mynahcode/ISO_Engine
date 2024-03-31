#include "iepch.h"
#include "IsoEngine/Renderer/Renderer3D.h"
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
		glm::vec3 Normal;
		float TextureIndex;

		// Editor-Only
		int EntityID;
	};


	struct Renderer3DStorage
	{
		static const uint32_t MAXVERTICES = 100000;
		static const uint32_t MAXINDICES = 100000;
		static const uint32_t MAXTEXTURESLOTS = 32; // TODO: RendererCapabilities class.

		Ref<VertexArray> VertexArray; // VAO
		Ref<VertexBuffer> VertexBuffer; // VBO

		Ref<Shader> ModelShader;
		Ref<Textures2D> WhiteTexture;

		uint32_t IndexCount = 0;
		QuadVertex* VertexBufferBase = nullptr;
		QuadVertex* VertexBufferPtr = nullptr;


		std::array<Ref<Textures2D>, MAXTEXTURESLOTS> TextureSlots;
		uint32_t TextureSlotIndex = 1; // Slot 0 dedicated to white texture.

		glm::vec4 VertexPositions;

		Renderer3D::Renderer3DStats Stats;


		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
	};


	static Renderer3DStorage s_Data3D;

	void Renderer3D::Init()
	{
		// TODO: Add reference counting system to ensure objects are only deleted AFTER all classes done using them.
		_IE_PROFILER_FUNCTION();
		ISOLOGGER_DEBUG("Renderer2D::Init() called.\n ");

		s_Data3D.VertexArray = VertexArray::Create();

		s_Data3D.VertexBuffer = VertexBuffer::Create(s_Data3D.MAXVERTICES * sizeof(QuadVertex));
		s_Data3D.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float2, "a_TexCoord"},
			{ ShaderDataType::Float3, "a_Normal"},
			{ ShaderDataType::Float, "a_TexIndex"},
			{ ShaderDataType::Int, "a_EntityID"}
			});

		s_Data3D.VertexArray->AddVertexBuffer(s_Data3D.VertexBuffer);

		s_Data3D.VertexBufferBase = new QuadVertex[s_Data3D.MAXVERTICES];

		uint32_t* quadIndices = new uint32_t[s_Data3D.MAXINDICES];

		Ref<IndexBuffer> m_quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data3D.MAXINDICES);
		s_Data3D.VertexArray->SetIndexBuffer(m_quadIndexBuffer);

		delete[] quadIndices;

		s_Data3D.WhiteTexture = Textures2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data3D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t)); // 4 bytes

		int32_t samplers[s_Data3D.MAXTEXTURESLOTS];
		for (uint32_t i = 0; i < s_Data3D.MAXTEXTURESLOTS; i++)
			samplers[i] = i;

		s_Data3D.ModelShader = Shader::Create("assets/shaders/Model.glsl");
		s_Data3D.ModelShader->Bind();
		s_Data3D.ModelShader->SetIntArray("u_Textures", samplers, s_Data3D.MAXTEXTURESLOTS);				// Texture slot that sampler samples from is slot 0.

		s_Data3D.TextureSlots[0] = s_Data3D.WhiteTexture;

		s_Data3D.VertexPositions = { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	void Renderer3D::Shutdown()
	{
		_IE_PROFILER_FUNCTION();

		delete[] s_Data3D.VertexBufferBase;
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		_IE_PROFILER_FUNCTION();

		//ISOLOGGER_TRACE("Renderer2D::BeginScene() with a Camera Component... \n");

		s_Data3D.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		s_Data3D.ModelShader->SetMat4("u_ViewProjection", viewProjection); // API agnostic call, in OpenGL its a Uniform, in DX it is setconstantbuffer
		StartBatch();
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		_IE_PROFILER_FUNCTION();

		//ISOLOGGER_TRACE("Renderer2D::BeginScene() with an Orthographic Camera... \n");
		s_Data3D.ModelShader->Bind();
		s_Data3D.ModelShader->SetMat4("u_ViewProjection", camera.GetVPMatrix()); // API agnostic call, in OpenGL its a Uniform, in DX it is setconstantbuffer

		StartBatch();
	}

	void Renderer3D::BeginScene(const PerspectiveEditorCamera& camera)
	{
		_IE_PROFILER_FUNCTION();

		//ISOLOGGER_TRACE("Renderer2D::BeginScene() with an IsoEngine Editor Camera... \n");
		s_Data3D.ModelShader->Bind();
		s_Data3D.ModelShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		StartBatch();
	}

	void Renderer3D::StartBatch()
	{
		s_Data3D.IndexCount = 0;
		s_Data3D.VertexBufferPtr = s_Data3D.VertexBufferBase;

		s_Data3D.TextureSlotIndex = 1;
	}

	void Renderer3D::EndScene()
	{
		_IE_PROFILER_FUNCTION();

		// Logging Bounded Texture Slots
		s_Data3D.Stats.InUseTextureSlots = s_Data3D.TextureSlotIndex;
		Flush();
	}

	void Renderer3D::Draw(uint32_t attachmentID)
	{
		s_Data3D.ModelShader->Bind();
		GLint location = glGetUniformLocation(s_Data3D.ModelShader->GetShaderID(), "u_Textures");
		glBindSampler(attachmentID, location);
		RenderCommand::DrawIndexed(s_Data3D.VertexArray, s_Data3D.IndexCount);
		s_Data3D.Stats.DrawCalls++;
	}

	void Renderer3D::Flush()
	{
		if (s_Data3D.IndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data3D.VertexBufferPtr - (uint8_t*)s_Data3D.VertexBufferBase);
		s_Data3D.VertexBuffer->SetData(s_Data3D.VertexBufferBase, dataSize);

		// Bind Textures
		for (uint32_t i = 0; i < s_Data3D.TextureSlotIndex; i++)
			s_Data3D.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data3D.VertexArray, s_Data3D.IndexCount);
		s_Data3D.Stats.DrawCalls++;
	}

	void Renderer3D::NextBatch()
	{
		ISOLOGGER_INFO("Renderer2D::NextBatch() called.\n")

			Flush();
		StartBatch();
	}


	void Renderer3D::ResetStats()
	{
		memset(&s_Data3D.Stats, 0, sizeof(Renderer3DStats));
	}

	Renderer3D::Renderer3DStats Renderer3D::GetStats()
	{
		return s_Data3D.Stats;
	}
}