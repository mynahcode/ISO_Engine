#pragma once

#include "IsoEngine/Renderer/OrthographicCamera.h"
#include "IsoEngine/Renderer/PerspectiveEditorCamera.h"
#include "IsoEngine/Renderer/Camera.h"
#include "IsoEngine/Renderer/Textures.h"
#include "IsoEngine/Renderer/SubTexture2D.h"
#include "IsoEngine/Scene/Components.h"

namespace IE
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const PerspectiveEditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Draw(uint32_t attachmentID);

		// Statistics
		struct Renderer3DStats
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
		static Renderer3DStats GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
		static void Flush();
	};
}