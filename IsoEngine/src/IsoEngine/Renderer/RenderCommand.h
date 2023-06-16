#pragma once

#include "IsoEngine/Renderer/RendererAPI.h"

namespace IE
{
	class RenderCommand
	{
	public:

		static void Init()
		{
			_IE_PROFILER_FUNCTION();

			s_RendererAPI->Init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		// RenderCommands should NOT do multiple things
		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}