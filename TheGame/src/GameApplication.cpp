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
		PushLayer(new TestLayer());
		PushOverlay(new IE::ImGuiLayer());
	}

	~TheGame()
	{

	}
};

IE::Application* IE::CreateApplication()
{
	return new TheGame();
}
