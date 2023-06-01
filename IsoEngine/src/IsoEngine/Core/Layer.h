#pragma once

#include "IsoEngine/Core/IsoMacros.h"
#include "IsoEngine/Events/Event.h"
#include "IsoEngine/Core/Timestep.h"

namespace IE
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {} // Should occur EVERY frame
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
