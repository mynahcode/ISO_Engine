#pragma once

#include "IsoEngine/IsoMacros.h"
#include "IsoEngine/Events/Event.h"

namespace IE
{
	class IsoEngine_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {} // Should occur EVERY frame
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}