#include <IsoEngine.h>

class TheGame : public IE::Application
{
public:
	TheGame()
	{

	}

	~TheGame()
	{

	}
};

/*
class ClientLogger : public IE::IELogger::IsoLogger
{
public:
	ClientLogger()
	{

	}

	~ClientLogger()
	{

	}
};
*/

IE::Application* IE::CreateApplication()
{
	return new TheGame();
}

/*
IE::IELogger::IsoLogger* IE::IELogger::IsoLogger_Init()
{
	return new ClientLogger();
}
*/