#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Textures.h"
#include "SubTexture2D.h"

typedef int GLint;

namespace IE
{
	class Material 
	{
	public:
		virtual ~Material() = default;
		virtual uint32_t GetShaderRendererID() const = 0;
		virtual void SetShaderUniforms() = 0;
	};

	class MaterialGL : public Material
	{
	public:
		//static Ref<Material2D> Create(const Ref<Shader>& shader, const Ref<Textures2D>& texture = nullptr);
		//static Ref<Material2D> Create(const Ref<Shader>& shader, const Ref<SubTexture2D>& subtexture = nullptr);
		//static Ref<Material2D> Create(const Ref<Shader>& shader, const Ref<Textures2D>& texture, Ref<SubTexture2D> subtexture = nullptr);
		static Ref<MaterialGL> Create(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const GLint& diffuseTex, const GLint& specularTex);
	};
}