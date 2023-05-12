#include <IsoEngine.h>

class TestLayer : public IE::Layer
{
public:
	TestLayer()
		: Layer("Test")
	{
	}

	void OnUpdate() override
	{

	}

	void OnEvent(IE::Event& event) override
	{
		
	}

};

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
