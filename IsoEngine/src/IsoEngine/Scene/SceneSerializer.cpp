#include "iepch.h"

#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
namespace IE
{
	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Begin serializing entity
		out << YAML::Key << "Entity" << YAML::Value << "12837192351342"; // TODO: Add entity ID's -- for now random pretend entity ID

		if (entity.HasComponent<TagComponent>()) // Serialize Entity's TagComponent
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Serializing Entity is finished.
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{

	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		// Go through every entity in registry, if entity is valid, then serialize it.
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);


			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{

	}

	void SceneSerializer::Deserialize(const std::string& filepath)
	{
		// Not Implemented Yet
		IE_ENGINE_ASSERT(false, "Not Implemented Yet.");
	}

	void SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not Implemented Yet
		IE_ENGINE_ASSERT(false, "Not Implemented Yet.");
	}
}