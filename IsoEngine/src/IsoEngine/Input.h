#pragma once

#include "IsoEngine/IsoMacros.h"

/* Global Input Polling Manager -- Singleton */
namespace IE
{
	class IsoEngine_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode);  } // TODO: Add IsoEngine's own keycodes so it can be platform independent.

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		inline static float GetMouseX(float xPos) { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY(float yPos) { return s_Instance->GetMouseYImpl(); }

	protected:
		/* Implemented Per - Platform */
		virtual bool IsKeyPressedImpl(int keycode) = 0; 

		virtual bool IsMouseButtonPressedImpl(int button) = 0; 

		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0; 
		virtual float GetMouseYImpl() = 0; 

	private:
		static Input* s_Instance; // Singleton Instance of Input
	};
}