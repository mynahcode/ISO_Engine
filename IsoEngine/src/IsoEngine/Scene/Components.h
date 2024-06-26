#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "IsoEngine/Core/IsoMacros.h"
#include "ScriptableEntity.h"
#include "SceneCamera.h"
#include "IsoEngine/Renderer/Material.h"

#include <vector>

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
		//Ref<Material> Material; // Material = Shader + (any) Uniform Data
		Ref<Textures2D> Texture;
		// TODO: Move to tile component to make sprite renderer functionality for tile rendering not
		// sound to the SpriteRendererComponent struct.
		//std::map<uint8_t, Ref<SubTexture2D>> SubTextures;
		std::vector<Ref<SubTexture2D>> SubTextures;
		float LightLevel = 0.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
		SpriteRendererComponent(const Ref<Textures2D>& texture)
			: Texture(texture) {}
		SpriteRendererComponent(const Ref<SubTexture2D>& subtexture) 
		{
			SubTextures.push_back(subtexture);
		}
		SpriteRendererComponent(const Ref<SubTexture2D>& subtexture, const Ref<SubTexture2D>& subtextureLayer)
		{
			SubTextures.push_back(subtexture);
			SubTextures.push_back(subtextureLayer);
		}
	};

	struct LightComponent
	{
		enum LightVariant
		{
			Diagonal = 1,
			InverseDiagonal = 2,
			Square = 3,
			Circular = 4,
		};

		glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
		LightVariant LightPattern = LightVariant::Diagonal;
		int Range = 5;

		LightComponent() = default;
		LightComponent(const LightComponent&) = default;
		LightComponent(int range)
			: Range(range) {}
		LightComponent(int range, glm::vec3 color)
			: Color(color), Range(range) {}
		LightComponent(int range, glm::vec3 color, LightVariant pattern)
			: Color(color), Range(range), LightPattern(pattern) {}

		int GetRange() const { return Range; }
		void SetRange(const int& range) { Range = range; }

		glm::vec3 GetColor() const { return Color; }
		void SetColor(const glm::vec3& color) { Color = color; }

		int GetLightPattern() const { return (int)LightPattern; }
		void SetLightPattern(int pattern) { LightPattern = (LightVariant)pattern; }

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
		ScreenQuadComponent(const ScreenQuadComponent& sqc) = default;
	};

	struct TileComponent
	{
		glm::vec2 Dimensions;
		bool IsSelected = false;

		TileComponent()
			: Dimensions({ 1.0f, 1.0f }) {}
		TileComponent(glm::vec2 dimensions)
			: Dimensions(dimensions) {}
		TileComponent(const TileComponent& tile) = default;

	};
}