#pragma once

#include "Entity.h"

namespace IE
{
	class ScriptableEntity : public Entity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	private:
		Entity m_Entity;
		friend class Scene; // Scene class can access private members/fns.
	};
}