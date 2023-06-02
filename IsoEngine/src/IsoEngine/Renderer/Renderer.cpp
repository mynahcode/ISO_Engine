#include "iepch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "IsoEngine/Platform/OpenGL/OpenGLShader.h"


namespace IE
{
	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height); // Set viewport from (0,0) to (width, height)
	}

	void Renderer::BeginScene(OrthographicCamera& camera) // TODO: Implement environment maps, cube maps, camera, projection matrix, view matrix, lighting as shader uniforms
	{
		m_SceneData->VPMatrix = camera.GetVPMatrix();
	}
	
	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		// TODO: Refactor to be API agnostic.
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->VPMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform); // needs to be done PER OBJECT

		//material_ir.Bind();

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}