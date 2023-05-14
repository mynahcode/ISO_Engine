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
		
		//ISOLOGGER_INFO("TestLayer::Update() called");
		
		if (IE::Input::IsKeyPressed(IE_KEY_TAB))
			ISOLOGGER_INFO("[Client]: Tab Key is Pressed! (POLL)");
		
	}

	void OnEvent(IE::Event& ev) override
	{
		
		if (ev.GetEventType() == IE::EventType::KeyPressed)
		{
			IE::KeyPressedEvent& event = (IE::KeyPressedEvent&)ev;
			if(event.GetKeyCode() == IE_KEY_TAB)
				ISOLOGGER_INFO("Tab key is pressed! (EVENT)");
			ISOLOGGER_INFO("KeyCode Char: %", (char)event.GetKeyCode());
		}
		
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
