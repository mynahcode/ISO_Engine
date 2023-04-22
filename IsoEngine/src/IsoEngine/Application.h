#pragma once

#include "IsoMacros.h"

namespace IE {

	class IsoEngine_API Application // export here, in game include.
	{
	public:
		Application();
		virtual ~Application(); // will be inherited by Game application
		
		void Run();
	};

	// Defined in client
	Application* CreateApplication();
}

