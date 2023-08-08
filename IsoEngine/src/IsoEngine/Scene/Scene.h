#pragma once

#include "entt.hpp"

namespace IE
{
	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		entt::registry m_Registry; // Contains entity component data and entity IDs --> Container that contains entity "context"
	};
}