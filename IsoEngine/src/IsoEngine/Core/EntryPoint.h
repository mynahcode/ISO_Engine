#pragma once

#include "IsoEngine/Core/IsoMacros.h"

#ifdef _IE_PLATFORM_WINDOWS

extern IE::Application* IE::CreateApplication();

int main(int argc, char** argv)
{
	_IE_PROFILER_BEGIN_SESSION("Startup", "IsoEngineProfile-Startup.json");
	auto app = IE::CreateApplication();
	_IE_PROFILER_END_SESSION();

	_IE_PROFILER_BEGIN_SESSION("Runtime", "IsoEngineProfile-Runtime.json");
	app->Run();
	_IE_PROFILER_END_SESSION();

	_IE_PROFILER_BEGIN_SESSION("Shutdown", "IsoEngineProfile-Shutdown.json");
	delete app;
	_IE_PROFILER_END_SESSION();
}

#endif