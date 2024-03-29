#pragma once

#include "IsoEngine/Renderer/OrthographicCamera.h"
#include "IsoEngine/Renderer/PerspectiveEditorCamera.h"
#include "IsoEngine/Renderer/Camera.h"
#include "IsoEngine/Renderer/Textures.h"
#include "IsoEngine/Renderer/SubTexture2D.h"

#include "IsoEngine/Scene/Components.h"

namespace IE
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const PerspectiveEditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Draw(uint32_t attachmentID);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);			// glm::vec3 for position, (x,y,z), x -> x_pos, y -> y_pos, z -> layering (drawing on top/blending textures)
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor);
		
		// ECS -- Drawing Quads via a Transform
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor, float lightLevel, int entityID = -1);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Textures2D>& texture, float tilingFactor, const glm::vec4& tintColor);		
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor);

		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, SpriteRendererComponent& src, int entityID);
		static void DrawIsometricSprite(const glm::vec3& position, const glm::vec2& size, SpriteRendererComponent& src, int entityID);

		// Statistics
		struct Renderer2DStats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t MaxTextureSlots = 32;
			uint32_t InUseTextureSlots = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
			uint32_t GetMaxTextureSlots() const { return MaxTextureSlots; }
			uint32_t GetTextureSlotsInUse() const { return InUseTextureSlots; }
		};

		static void ResetStats();
		static Renderer2DStats GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
		static void Flush();
	};
}