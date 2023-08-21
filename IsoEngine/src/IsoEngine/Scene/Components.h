#pragma once

#include <glm/glm.hpp>

#include "IsoEngine/Renderer/Camera.h"

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
		Camera Camera;
		bool isPrimary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}
	};
}