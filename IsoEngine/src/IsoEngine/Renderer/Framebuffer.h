#pragma once

#include "IsoEngine/Core/IsoMacros.h"

namespace IE
{
	struct FramebufferSpecs
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false; // glBindBuffer(0)
	};

	class Framebuffer
	{
	public: 
		virtual const FramebufferSpecs& GetFramebufferSpecs() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecs& specs);

	};
}