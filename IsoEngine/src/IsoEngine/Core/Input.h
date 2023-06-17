#pragma once

#include "IsoEngine/Core/IsoMacros.h"

/* Global Input Polling Manager -- Singleton Instance */
namespace IE
{
	class Input
	{
	public:
		virtual ~Input() = default;
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode);  } // TODO: Add IsoEngine's own keycodes so it can be platform independent.

		static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		static float GetMouseX(float xPos) { return s_Instance->GetMouseXImpl(); }
		static float GetMouseY(float yPos) { return s_Instance->GetMouseYImpl(); }

	protected:
		/* Implemented Per - Platform */
		Input() = default;
		virtual bool IsKeyPressedImpl(int keycode) = 0; 

		virtual bool IsMouseButtonPressedImpl(int button) = 0; 

		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0; 
		virtual float GetMouseYImpl() = 0; 

	private:
		static Scope<Input> s_Instance;
	};
}