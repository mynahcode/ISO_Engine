#pragma once

#include "IsoEngine/Core/IsoMacros.h"

namespace IE
{
	enum class fbTextureFormats
	{
		None = 0,
		RGBA8,
		DEPTH24STENCIL8,

		// Default configuration
		RGB = RGBA8,
		Depth = DEPTH24STENCIL8
	};


	struct FramebufferSpecs
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false; // glBindBuffer(0)
	};

	class Framebuffer
	{
	public: 
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpecs& GetFramebufferSpecs() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecs& specs);

	};
}