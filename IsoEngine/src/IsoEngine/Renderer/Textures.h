#pragma once

#include "IsoEngine/Core/IsoMacros.h"

#include <string>

namespace IE
{
	class Textures
	{
	public:
		virtual ~Textures() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;	// Creates pointer to a block in memory that gets uploaded to the GPU.

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Textures& other) const = 0;
	};

	class Textures2D : public Textures
	{
	public:
		static Ref<Textures2D> Create(uint32_t width, uint32_t height);
		static Ref<Textures2D> Create(const std::string& path);

	};
}