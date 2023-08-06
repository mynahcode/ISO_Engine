#pragma once

#include "IsoEngine/Core/IsoMacros.h"
#include "IsoEngine/Core/KeyCodes.h"
#include "IsoEngine/Core/MouseCodes.h"

/* Global Input Polling Manager -- Singleton Instance */
namespace IE
{
	class Input
	{
	public:

		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};
}