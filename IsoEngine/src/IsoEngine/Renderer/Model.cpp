#include "iepch.h"

#include "Model.h"

namespace IE
{
	Model::Model(const glm::vec3& position, const Ref<Material>& material, const Ref<Textures2D>& textureDiffuse, const Ref<Textures2D>& textureSpecular, const std::vector<Ref<Mesh>>& meshes)
		:m_Position(position), m_Material(material), m_OverrideTextureDiffuse(textureDiffuse), m_OverrideTextureSpecular(textureSpecular), m_Meshes(meshes)
	{

	}

	void Model::UpdateUniforms()
	{

	}
}