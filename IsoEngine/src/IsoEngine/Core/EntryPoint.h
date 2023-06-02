#pragma once

#include "IsoEngine/Core/IsoMacros.h"

#ifdef _IE_PLATFORM_WINDOWS

extern IE::Application* IE::CreateApplication();

int main(int argc, char** argv)
{
	auto app = IE::CreateApplication();
	app->Run();
	delete app;
}

#endif