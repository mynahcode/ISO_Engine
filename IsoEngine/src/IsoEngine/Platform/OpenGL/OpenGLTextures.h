#pragma once

#include "IsoEngine/Renderer/Textures.h"

namespace IE
{
	class OpenGLTextures2D : public Textures2D
	{
	public:
		OpenGLTextures2D(const std::string& path);
		virtual ~OpenGLTextures2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;


	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
	};

}