#pragma once

#include "IsoEngine/Renderer/Textures.h"
#include "IsoEngine/Renderer/Shader.h"
#include "IsoEngine/Renderer/Material.h"
#include "IsoEngine/Renderer/Mesh.h"

namespace IE
{
	class Model
	{
	public:
		Model(const glm::vec3& position, const Ref<Material>& material, const Ref<Textures2D>& textureDiffuse, const Ref<Textures2D>& textureSpecular, const std::vector<Ref<Mesh>>& meshes);

	private:
		void UpdateUniforms();

		Ref<Material> m_Material;
		Ref<Textures2D> m_OverrideTextureDiffuse;
		Ref<Textures2D> m_OverrideTextureSpecular;
		std::vector<Ref<Mesh>> m_Meshes;
		glm::vec3 m_Position;
	};
}