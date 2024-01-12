#include "iepch.h"

#include "TextureManager.h"

namespace IE
{
	TextureManager::TextureManager(const Ref<Textures2D>& spriteSheet, const glm::vec2& sheetDimensions, const glm::vec2& spriteDimensions, const glm::vec2& sheetRowsandCols)
	{
		for (int i = 0; i < sheetRowsandCols.x; i++)
		{
			for (int j = 0; j < sheetRowsandCols.y; j++)
			{
				glm::vec2 coords = { (float)i * spriteDimensions.x, (float)j * spriteDimensions.y };
				Ref<SubTexture2D> subTexture;
				subTexture->CreateFromCoords(spriteSheet, coords, spriteDimensions);
				m_Sprites.push_back(subTexture);
			}
		}
	}
}