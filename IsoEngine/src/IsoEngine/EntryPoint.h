#pragma once

#include "IsoEngine/IsoLogger/IsoLogger.h"

#ifdef _WIN32

extern IE::Application* IE::CreateApplication();

int main(int argc, char** argv)
{
	auto app = IE::CreateApplication();
	app->Run();
	delete app;
}

#endif