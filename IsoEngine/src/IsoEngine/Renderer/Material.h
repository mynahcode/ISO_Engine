#pragma once

#include "glm/glm.hpp"

#include "Shader.h"
#include "Textures.h"
#include "SubTexture2D.h"

namespace IE
{
	class Material 
	{
	public:
		virtual ~Material() = default;
		virtual uint32_t GetShaderRendererID() const = 0;
		virtual void SetShaderUniforms() = 0;
	};

	class Material2D : public Material
	{
	public:
		static Ref<Material2D> Create(const Ref<Shader>& shader, const Ref<Textures2D>& texture = nullptr);
		static Ref<Material2D> Create(const Ref<Shader>& shader, const Ref<SubTexture2D>& subtexture = nullptr);
		static Ref<Material2D> Create(const Ref<Shader>& shader, const Ref<Textures2D>& texture, Ref<SubTexture2D> subtexture = nullptr);
	};
}