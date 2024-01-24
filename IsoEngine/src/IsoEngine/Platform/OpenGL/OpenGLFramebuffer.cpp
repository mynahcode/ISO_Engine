#include "iepch.h"
#include "OpenGLFramebuffer.h"
#include "IsoEngine/Renderer/RenderCommand.h"
#include <glad/glad.h>

namespace IE {

	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;

			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(fbTextureFormats format)
		{
			switch (format)
			{
				case fbTextureFormats::DEPTH24STENCIL8:  return true;
				case fbTextureFormats::DEPTHCOMPONENT32: return true;
			}

			return false;
		}

		static GLenum IEFramebufferTextureFormatToGL(fbTextureFormats format)
		{
			switch (format)
			{
				case fbTextureFormats::RGBA8:
					return GL_RGBA8;

				case fbTextureFormats::RED_INTEGER:
					return GL_RED_INTEGER;
			}
			IE_ENGINE_ASSERT(false, "No framebuffertextureformat given!");
			return 0;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& specs)
		: m_FramebufferSpecs(specs)
	{
		ISOLOGGER_TRACE("OpenGLFramebuffer Constructor Called...\n");

		for (auto specs : m_FramebufferSpecs.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(specs.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(specs);
			else
				m_DepthAttachmentSpecification = specs;
		}

		multisample = m_FramebufferSpecs.Samples > 1;
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		ISOLOGGER_TRACE("OpenGLFramebuffer deconstructor called...\n");

		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		ISOLOGGER_TRACE("Framebuffer::Invalidate() called...\n");

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_FramebufferSpecs.Samples > 1;

		// Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case fbTextureFormats::RGBA8:
					ISOLOGGER_TRACE("RGBA8 color attachment determined...\n");
					Utils::AttachColorTexture(m_ColorAttachments[i], m_FramebufferSpecs.Samples, GL_RGBA8, GL_RGBA, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height, i);
					break;
				case fbTextureFormats::RED_INTEGER:
					ISOLOGGER_TRACE("RED_INTEGER color attachment determined...\n");
					Utils::AttachColorTexture(m_ColorAttachments[i], m_FramebufferSpecs.Samples, GL_R32I, GL_RED_INTEGER, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height, i);
					break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != fbTextureFormats::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case fbTextureFormats::DEPTH24STENCIL8:
				ISOLOGGER_TRACE("GL_Depth24_Stencil8 Depth Attachment texture format determined...\n")
				Utils::AttachDepthTexture(m_DepthAttachment, m_FramebufferSpecs.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);
				break;

			case fbTextureFormats::DEPTHCOMPONENT32:
				ISOLOGGER_TRACE("GL_DEPTH_COMPONENT32 Depth Attachment texture format determined...\n")
				Utils::AttachDepthTexture(m_DepthAttachment, m_FramebufferSpecs.Samples, GL_DEPTH_COMPONENT32, GL_DEPTH_STENCIL_ATTACHMENT, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);
				break;
			}
		}

		Utils::BindTexture(multisample, 0);

		ISOLOGGER_WARN("Color attachment size: {0} \n", m_ColorAttachments.size());
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		if (m_ColorAttachments.size() > 1)
		{
			IE_ENGINE_ASSERT((m_ColorAttachments.size() <= 4), "Error, too many color attachments\n");
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		IE_ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == (GL_FRAMEBUFFER_COMPLETE), "Framebuffer is incomplete! \n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			ISOLOGGER_WARN("Attempted to resize framebuffer size to {0}, {1}... \n", width, height)
			return;
		}

		m_FramebufferSpecs.Width = width;
		m_FramebufferSpecs.Height = height;

		Invalidate();
	}

	void OpenGLFramebuffer::Bind()
	{
		//ISOLOGGER_WARN("(GL) OpenGLFramebuffer::Bind() called.\n");
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

	}

	void OpenGLFramebuffer::UnBind()
	{
		//ISOLOGGER_WARN("(GL) OpenGLFramebuffer::UnBind() called.\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		RenderCommand::Clear();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		IE_ENGINE_ASSERT(!(attachmentIndex >= m_ColorAttachments.size()), "Error attachment index out of range. \n");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		IE_ENGINE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "AttachmentIndex > m_ColorAttachments max index \n");

		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
		Utils::IEFramebufferTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}
}