#include "iepch.h"
#include "OpenGLTextures.h"
#include "stb_image.h"

#include <glad/glad.h>

namespace IE
{

	OpenGLTextures2D::OpenGLTextures2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* imgData = stbi_load(path.c_str(), &width, &height, &channels, 0);
		//IE_CORE_ASSERT(imgData, "Failed to load image texture");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		//IE_CORE_ASSERT(internalFormat & dataFormat, "Texture image format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);		// GL_LINEAR may be best for image textures with lots of different colors.

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);			// Handles wrapping textures, default is GL_REPEAT
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);			// GL_REPEAT ensures the textures is TILED

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, imgData);

		stbi_image_free(imgData);
	}

	OpenGLTextures2D::~OpenGLTextures2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextures2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}