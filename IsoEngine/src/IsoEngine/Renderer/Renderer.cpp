#include "iepch.h"
#include "Renderer.h"

namespace IE
{
	void Renderer::BeginScene() // TODO: Implement environment maps, cube maps, camera, projection matrix, view matrix, lighting as shader uniforms
	{
	}
	
	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}