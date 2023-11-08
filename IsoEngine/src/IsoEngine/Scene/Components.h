#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		// TODO: Ref<MaterialInstance> Material; // Material = Shader + (any) Uniform Data

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		uint32_t Width, Height;
		SceneCamera Camera;
		bool isPrimary = true;
		bool fixedAspectRatio = false;

		CameraComponent()
			: Width(1920), Height(1080), Camera(1920, 1080, SceneCamera::ProjectionType::Isometric) {}
		CameraComponent(uint32_t width, uint32_t height)
			: Width(width), Height(height), Camera(width, height, SceneCamera::ProjectionType::Isometric) {}
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

	struct ScreenQuadComponent
	{
		uint32_t Width, Height;
		Ref<Textures2D> ScreenTexture;
		ScreenQuadComponent(uint32_t width, uint32_t height)
			: Width(width), Height(height) {}
		ScreenQuadComponent(const ScreenQuadComponent&) = default;
	};
}