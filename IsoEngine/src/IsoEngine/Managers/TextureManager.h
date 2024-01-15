#pragma once

#include "IsoEngine/Renderer/Textures.h"
#include "IsoEngine/Renderer/SubTexture2D.h"

namespace IE
{
	class TextureManager
	{
	public:
		// Texture Manager Constructor -- textures should be loaded in through the manager as spritesheets.
		TextureManager() = default;
		TextureManager(const Ref<Textures2D>& spriteSheet, const glm::vec2& sheetDimensions, const glm::vec2& spriteDimensions, const glm::uvec2& sheetRowsandCols);

		~TextureManager() = default;

		std::vector<Ref<SubTexture2D>> GetTextureSprites() const { return m_Sprites; }

	private:
		std::vector<Ref<SubTexture2D>> m_Sprites;

	};
}