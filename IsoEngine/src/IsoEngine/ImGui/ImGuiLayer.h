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
		virtual void OnImGuiRender() override;
		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}