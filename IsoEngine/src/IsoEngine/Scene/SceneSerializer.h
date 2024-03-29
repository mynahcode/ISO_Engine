#pragma once

#include "IsoEngine/Core/IsoMacros.h"

#include "Scene.h"

namespace IE
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath); // Serialize to YAML
		void SerializeRuntime(const std::string& filepath); // Serialize to Binary

		bool Deserialize(const std::string& filepath); // Deserialize from YAML
		bool DeserializeRuntime(const std::string& filepath); // Deserialize from Binary

	private:
		Ref<Scene> m_Scene;
	};
}