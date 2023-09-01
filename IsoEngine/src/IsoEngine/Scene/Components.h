#pragma once

#include <glm/glm.hpp>

#include "ScriptableEntity.h"
#include "SceneCamera.h"

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
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}
		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
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

		std::function<void()> InstantiateFunc;
		std::function<void()> DestroyInstanceFunc;
		std::function<void(ScriptableEntity*)> OnCreateFunc;
		std::function<void(ScriptableEntity*)> OnDestroyFunc;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunc;

		template<typename T>
		void Bind()
		{
			InstantiateFunc = [&]() { Instance = new T(); };
			DestroyInstanceFunc = [&]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunc = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnDestroyFunc = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
			OnUpdateFunc = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(ts); };
		}
	};
}