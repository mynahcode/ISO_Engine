#pragma once

#include "IsoEngine/Renderer/OrthographicCamera.h"
#include "IsoEngine/Renderer/Textures.h"

namespace IE
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// TODO: Refactor drawing to be api agnostic.
		/* Primitives */
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);					//glm::vec3 for position, (x,y,z), x -> x_pos, y -> y_pos, z -> layering (drawing on top/blending textures)
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor);
		//static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Textures2D>& texture, const glm::vec4& color);	// Renders Quad with varying RGBA for effects such as fading/tinting.
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor);
	};
}