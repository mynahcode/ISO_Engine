#pragma once

#include "IsoEngine/Core/IsoMacros.h"
#include "IsoEngine/Core/Layer.h"

#include "IsoEngine/Events/KeyEvent.h"
#include "IsoEngine/Events/MouseEvent.h"
#include "IsoEngine/Events/ApplicationEvent.h"

namespace IE
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnEvent(Event& e);
		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}