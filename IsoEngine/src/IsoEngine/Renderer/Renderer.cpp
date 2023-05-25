#include "iepch.h"
#include "Renderer.h"
#include "IsoEngine/Platform/OpenGL/OpenGLShader.h"


namespace IE
{
	Renderer::SceneData* Renderer::m_SceneData = new SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
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
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->VPMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform); // needs to be done PER OBJECT

		//material_ir.Bind();

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}