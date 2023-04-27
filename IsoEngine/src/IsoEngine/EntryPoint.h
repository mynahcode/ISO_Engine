#pragma once

#include "IsoEngine/IsoLogger/IsoLogger.h"
#include <thread>

#ifdef _WIN32

extern IE::Application* IE::CreateApplication();

int main(int argc, char** argv)
{
	IE::IELogger::IsoLogger::EnableIsoFileOut("logs/testlog.txt");

	//std::thread threads[10];

	//for (int i = 0; i < 10; i++) { threads[i] = std::thread(log_test, i); }
	//for (int i = 0; i < 10; i++) { threads[i].join(); }

	const char* name = "User";
	ISOLOGGER_INFO("Hello %s", name);

	auto app = IE::CreateApplication();
	app->Run();
	delete app;
}

#endif