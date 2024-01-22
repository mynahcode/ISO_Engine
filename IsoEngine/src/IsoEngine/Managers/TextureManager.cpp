#include "iepch.h"

#include "TextureManager.h"

namespace IE
{
	// Reads NxM quad Sprite Textures from a Spritesheet starting from the bottom left corner to the
	// top-right corner of the Spritesheet.
	TextureManager::TextureManager(const Ref<Textures2D>& spriteSheet, const glm::vec2& sheetDimensions, const glm::vec2& spriteDimensions, const glm::uvec2& sheetRowsandCols)
	{
		for (int i = 0; i < sheetRowsandCols.y; i++)
		{
			for (int j = 0; j < sheetRowsandCols.x; j++)
			{
				glm::vec2 coords = { (float)i, (float)j};
				Ref<SubTexture2D> subTexture;
				ISOLOGGER_WARN("Creating SubTexture at coords <{0}, {1}>\n", coords.x, coords.y);
				auto texture = subTexture->CreateFromCoords(spriteSheet, coords, spriteDimensions);
				m_Sprites.push_back(texture);
			}
		}
	}
}