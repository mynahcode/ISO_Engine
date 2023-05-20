#include <IsoEngine.h>

#include "imgui/imgui.h"

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
		{
			//IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::DEBUG);
			//ISOLOGGER_INFO("[Client]: Tab Key is Pressed! (POLL)");
		}
		
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		//ImGui::Text("Hello World!");
		ImGui::End();
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
	}

	~TheGame()
	{

	}
};

IE::Application* IE::CreateApplication()
{
	return new TheGame();
}
