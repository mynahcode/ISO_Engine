#pragma once

#include "IsoEngine/IsoMacros.h"
#include "IsoEngine/Layer.h"

#include "IsoEngine/Events/KeyEvent.h"
#include "IsoEngine/Events/MouseEvent.h"
#include "IsoEngine/Events/ApplicationEvent.h"

namespace IE
{
	class IsoEngine_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();
		virtual void OnEvent(Event& event);

	private:
		/* IsoEngine ImGui Mouse Events */
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& ev);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& ev);
		bool OnMouseMovedEvent(MouseMovedEvent& ev);
		bool OnMouseScrolledEvent(MouseScrolledEvent& ev);

		/* IsoEngine ImGui Key Events */
		bool OnKeyTypedEvent(KeyTypedEvent& ev);
		bool OnKeyPressedEvent(KeyPressedEvent& ev);
		bool OnKeyReleasedEvent(KeyReleasedEvent& ev);

		/* IsoEngine ImGui Window Events */
		bool OnWindowResizeEvent(WindowResizeEvent& ev);

		float m_Time = 0.0f;
	};
}