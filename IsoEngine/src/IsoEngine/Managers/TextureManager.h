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
		TextureManager(const Ref<Textures2D>& spriteSheet, const glm::vec2& sheetDimensions, const glm::vec2& spriteDimensions, const glm::vec2& sheetRowsandCols);

		~TextureManager() = default;

	private:
		std::vector<Ref<SubTexture2D>> m_Sprites;

	};
}