#pragma once

#include "IsoEngine/Scene/Entity.h"
#include "IsoEngine/Core/Timestep.h"

namespace IE
{
	class ScriptableEntity : public Entity
	{
	public:
		virtual ~ScriptableEntity() {};

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(Timestep ts) {};

	private:
		Entity m_Entity;
		friend class Scene; // Scene class can access private members/fns.
	};
}