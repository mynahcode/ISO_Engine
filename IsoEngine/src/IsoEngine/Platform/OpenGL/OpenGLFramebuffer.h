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

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override 
		{ //IE_ENGINE_ASSERT(index < m_ColorAttachments.size()); 
			return m_ColorAttachments[index]; }

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecs m_FramebufferSpecs;

		std::vector<FramebufferTextureSpecs> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecs m_DepthAttachmentSpecification = fbTextureFormats::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}