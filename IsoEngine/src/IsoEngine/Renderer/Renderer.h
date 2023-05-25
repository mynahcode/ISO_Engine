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

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 VPMatrix;
		};

		static SceneData* m_SceneData;
	};
}