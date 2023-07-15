#pragma once

#include "IsoEngine/Renderer/Textures.h"

#include <glad/glad.h>

namespace IE
{
	class OpenGLTextures2D : public Textures2D
	{
	public:
		OpenGLTextures2D(uint32_t width, uint32_t height);
		OpenGLTextures2D(const std::string& path);
		virtual ~OpenGLTextures2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Textures& other) const override { return m_RendererID == ((OpenGLTextures2D&)other).m_RendererID; };
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

}