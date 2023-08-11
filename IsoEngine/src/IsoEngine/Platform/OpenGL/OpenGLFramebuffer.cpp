#include "iepch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace IE {

	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& specs)
		: m_FramebufferSpecs(specs)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); // TODO: GL_RGBA32F (HDR Support)

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		IE_ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FRAMEBUFFER is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			ISOLOGGER_WARN("Attempted to resize framebuffer size to {0}, {1}", width, height)
			return;
		}

		m_FramebufferSpecs.Width = width;
		m_FramebufferSpecs.Height = height;

		Invalidate();
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);
	}

	void OpenGLFramebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}