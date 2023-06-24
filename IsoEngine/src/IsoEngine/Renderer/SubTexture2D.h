#pragma once

#include <glm/glm.hpp>

#include "IsoEngine/Core/IsoMacros.h"
#include "Textures.h"

namespace IE
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Textures2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Textures2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTextureCoords() const { return m_TextureCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Textures2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });

	private:
		Ref<Textures2D> m_Texture;
		glm::vec2 m_TextureCoords[4];
	};
}