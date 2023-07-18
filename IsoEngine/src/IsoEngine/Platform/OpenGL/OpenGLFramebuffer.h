#pragma once

#include "IsoEngine/Renderer/Framebuffer.h"

namespace IE
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecs& specs);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual const FramebufferSpecs& GetFramebufferSpecs() const override { return m_FramebufferSpecs; }
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecs m_FramebufferSpecs;
	};
}