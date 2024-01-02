#pragma once

#include "IsoEngine/Core/IsoMacros.h"

namespace IE
{
	enum class fbTextureFormats
	{
		None = 0,
		RGBA8,
		RED_INTEGER,
		DEPTH24STENCIL8,

		// Default configuration
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecs
	{
		FramebufferTextureSpecs() = default;
		FramebufferTextureSpecs(fbTextureFormats format)
			: TextureFormat(format) {}

		fbTextureFormats TextureFormat = fbTextureFormats::None;

	};

	struct FramebufferAttachmentSpecs
	{
		FramebufferAttachmentSpecs() = default;
		FramebufferAttachmentSpecs(std::initializer_list<FramebufferTextureSpecs> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecs> Attachments;
	};

	struct FramebufferSpecs
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false; // glBindBuffer(0)

		FramebufferAttachmentSpecs Attachments;
	};

	class Framebuffer
	{
	public: 
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpecs& GetFramebufferSpecs() const = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecs& specs);

	};
}