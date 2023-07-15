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

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual const FramebufferSpecs& GetFramebufferSpecs() const override { return m_FramebufferSpecs; }
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecs m_FramebufferSpecs;
	};
}