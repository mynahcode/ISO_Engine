#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "IsoEngine/Core/IsoMacros.h"
#include "ScriptableEntity.h"
#include "SceneCamera.h"
#include "IsoEngine/Renderer/Textures.h"

namespace IE
{
	/* Name component for entities, if none is given upon instantiation of the TagComponent a default will be given.
	 * Not the same as the entity's ID component which will be added.
	 */
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
								* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
								* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		//int TilingFactor;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		// TODO: Ref<MaterialInstance> Material; // Material = Shader + (any) Uniform Data
		Ref<Textures2D> Texture;

		SpriteRendererComponent() = default;
		/* Copy Constructor */
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
		SpriteRendererComponent(const Ref<Textures2D>& texture)
			: Texture(texture) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool isPrimary = true;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)(); // func pointer
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); }; // Capturing lambdas
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	/* Temporary maybe */
	struct HealthComponent
	{
		int Health;
		int Armor;

		HealthComponent()
			: Health(3), Armor(5) {}
		HealthComponent(int health, int armor)
			: Health(health), Armor(armor) {}
		HealthComponent(const HealthComponent&) = default;
		HealthComponent(const int& health, const int& armor)
			: Health(health), Armor(armor) {}
	};
}