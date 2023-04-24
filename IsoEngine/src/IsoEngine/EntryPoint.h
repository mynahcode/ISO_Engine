#pragma once
//#include "IsoEngine/IsoLogger.h"
#include "IsoEngine/IsoLogger/IsoLogger.h"
#include <thread>

#ifdef _WIN32

extern IE::Application* IE::CreateApplication();
//extern IE::IELogger::IsoLogger* IE::IELogger::IsoLogger_Init();

void log_test(int n)
{
	IE::IELogger::IsoLogger::Info("%d", n);
}

int main(int argc, char** argv)
{
	/*
	IE::IsoLogger::Init();
	IE::IsoLogger::GetCoreLogger()->warn("Initialized Dev-Side Logger");
	IE::IsoLogger::GetClientLogger()->info("Initialized Client-Side Logger");
	*/
	
	std::thread threads[10];

	for (int i = 0; i < 10; i++) { threads[i] = std::thread(log_test, i); }
	for (int i = 0; i < 10; i++) { threads[i].join(); }

	auto app = IE::CreateApplication();
	app->Run();
	delete app;
}

#endif