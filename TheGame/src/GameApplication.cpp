#include <IsoEngine.h>
#include "IsoEngine/Events/ApplicationEvent.h"
#include "IsoEngine/IsoLogger/IsoLogger.h"

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

IE::Application* IE::CreateApplication()
{
	return new TheGame();
}
