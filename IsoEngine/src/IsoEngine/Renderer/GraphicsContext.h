#pragma once

#include "IsoEngine/Core/IsoMacros.h"

namespace IE
{
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};
}