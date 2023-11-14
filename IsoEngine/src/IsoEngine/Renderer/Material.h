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
		//static Ref<TileMaterial> Create(uint32_t width, uint32_t height); //Empty Tile with default texture
		//static Ref<TileMaterial> Create(uint32_t width, uint32_t height, uint32_t renderID);
		//static Ref<TileMaterial> Create(Ref<Textures2D> TileTexture, Ref<Shader>);
		//static Ref<TileMaterial> Create(Ref<SubTexture2D> TileTexture, Ref<Shader>);
	};
}