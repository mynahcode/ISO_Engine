#include "iepch.h"
#include "Renderer.h"

namespace IE
{
	Renderer::SceneData* Renderer::m_SceneData = new SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera) // TODO: Implement environment maps, cube maps, camera, projection matrix, view matrix, lighting as shader uniforms
	{
		m_SceneData->VPMatrix = camera.GetVPMatrix();
	}
	
	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->VPMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}