#pragma once

#include "RenderCommand.h"

namespace IE
{
	class Renderer
	{
	public:
		static void BeginScene(); // TODO: Add scene parameters: lighting, camera, environment
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}