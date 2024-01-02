#pragma once

#include "IsoEngine/Core/IsoMacros.h"


#include "Textures.h"
#include "SubTexture2D.h"
#include "Shader.h"

namespace IE
{
	class Material 
	{
		//virtual ~Material() = default;
		virtual uint32_t GetMaterialRendererID() const = 0;

		virtual bool operator==(const Material& other) const = 0;
	};


	class TileMaterial : public Material
	{
	public:
		static Ref<TileMaterial> Create(uint32_t width, uint32_t height, Ref<Textures2D> tileTexture, Ref<Shader> shader);
		static Ref<TileMaterial> Create(uint32_t width, uint32_t height, Ref<SubTexture2D> tileTexture, Ref<Shader> shader);
	};
}