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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}