#include "iepch.h"

#include "Entity.h"

namespace IE
{
	Entity::Entity(entt::entity e_Handle, Scene* scene)
		: m_EntityHandle(e_Handle), m_Scene(scene)
	{
	}
}