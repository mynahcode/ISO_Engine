#pragma once

#ifdef IE_PLATFORM_WINDOWS
extern IE::Application* IE::CreateApplication();

	// WinMain
int main(int argc, char** argv)
{
	auto app = IE::CreateApplication();
	printf("IsoEngine Linked Successfully to Application!");
	app->Run();
	delete app;
}

#endif