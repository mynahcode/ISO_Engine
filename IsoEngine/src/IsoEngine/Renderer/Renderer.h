#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace IE
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera); // TODO: Add scene parameters: lighting, camera, environment
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 VPMatrix;
		};

		static SceneData* m_SceneData;
	};
}