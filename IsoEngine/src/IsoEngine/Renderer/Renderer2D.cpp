#include "iepch.h"
#include "IsoEngine/Renderer/Renderer2D.h"

#include "IsoEngine/Renderer/VertexArray.h"
#include "IsoEngine/Renderer/Shader.h"
#include "IsoEngine/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IE
{

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Textures2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data2D;

	void Renderer2D::Init()
	{
		_IE_PROFILER_FUNCTION();

		s_Data2D = new Renderer2DStorage();

		/* Rendering a Square */
		s_Data2D->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,				// Texture coords: Bottom Left (0.0f, 0.0f)
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,				// Texture coords: Bottom Right (1.0f, 0.0f)
			 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,				// Texture coords: Top Right (1.0f, 1.0f)
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f				// Texture coords: Top Left (0.0f, 1.0f)
		};

		Ref<VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_SquareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float2, "a_TextureCoord"}
			});
		s_Data2D->QuadVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data2D->QuadVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

		s_Data2D->WhiteTexture = Textures2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data2D->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t)); // 4 bytes

		s_Data2D->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data2D->TextureShader->Bind();
		s_Data2D->TextureShader->SetInt("u_Texture", 0);				// Texture slot that sampler samples from is slot 0.
	}

	void Renderer2D::Shutdown()
	{
		_IE_PROFILER_FUNCTION();

		delete s_Data2D;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		_IE_PROFILER_FUNCTION();

		s_Data2D->TextureShader->Bind();
		s_Data2D->TextureShader->SetMat4("u_ViewProjection", camera.GetVPMatrix()); // API agnostic call, in OpenGL its a Uniform, in DX it is setconstantbuffer
	}

	void Renderer2D::EndScene()
	{
		_IE_PROFILER_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		_IE_PROFILER_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		_IE_PROFILER_FUNCTION();

		s_Data2D->TextureShader->SetFloat4("u_Color", color);
		s_Data2D->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data2D->WhiteTexture->Bind();

		/* Calculate the transform matrix */
		// TODO: Add rotation matrix to the calculation for transform.
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); // transform matrix = translation matrix * rotation matrix * scale matrix (TRS), scaling done along x and y axes not z.
		s_Data2D->TextureShader->SetMat4("u_Transform", transform); // Must be set on a per-draw basis

		/* Draw Call */
		s_Data2D->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data2D->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor)
	{
		_IE_PROFILER_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor)
	{
		_IE_PROFILER_FUNCTION();

		s_Data2D->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data2D->TextureShader->SetFloat("u_TilingFactor", tilingFactor); 
		texture->Bind();

		/* Calculate the transform matrix */
		// TODO: Add rotation matrix to the calculation for transform.
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); // transform matrix = translation matrix * rotation matrix * scale matrix (TRS), scaling done along x and y axes not z.
		s_Data2D->TextureShader->SetMat4("u_Transform", transform); // Must be set on a per-draw basis

		/* Draw Call */
		s_Data2D->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data2D->QuadVertexArray);
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

		s_Data2D->TextureShader->SetFloat4("u_Color", color);
		s_Data2D->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data2D->WhiteTexture->Bind();

		/* Calculate the transform matrix */
		// TODO: Add rotation matrix to the calculation for transform.
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); // transform matrix = translation matrix * rotation matrix * scale matrix (TRS), scaling done along x and y axes not z.
		s_Data2D->TextureShader->SetMat4("u_Transform", transform); // Must be set on a per-draw basis

		/* Draw Call */
		s_Data2D->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data2D->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor)
	{
		_IE_PROFILER_FUNCTION();

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor)
	{
		_IE_PROFILER_FUNCTION();

		s_Data2D->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data2D->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		/* Calculate the transform matrix */
		// TODO: Add rotation matrix to the calculation for transform.
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }); // transform matrix = translation matrix * rotation matrix * scale matrix (TRS), scaling done along x and y axes not z.
		s_Data2D->TextureShader->SetMat4("u_Transform", transform); // Must be set on a per-draw basis

		/* Draw Call */
		s_Data2D->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data2D->QuadVertexArray);
	}
}