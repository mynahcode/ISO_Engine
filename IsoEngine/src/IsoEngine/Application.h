#pragma once

#include "IsoEngine/IsoMacros.h"

int main(int argc, char** argv);

namespace IE 
{

	class IsoEngine_API Application // export here, in game include.
	{
	public:
		Application();
		virtual ~Application(); // will be inherited by Game application

		void Run();
	};

	// Defined in client
	//Application* CreateApplication();
	Application* CreateApplication()
	{
		return new Application();
	}
}
