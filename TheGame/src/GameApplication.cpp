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

IE::Application* IE::CreateApplication()
{
	return new TheGame();
}
