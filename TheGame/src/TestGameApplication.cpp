#include <IsoEngine.h>
#include <IsoEngine/Core/EntryPoint.h>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TestGameApplication2D.h"

class TestGame : public IE::Application
{
public:
	TestGame()
		: Application("TestGame")
	{
		//PushLayer(new TestLayer());
		PushLayer(new TestGame2D());
	}

	~TestGame()
	{

	}
};

IE::Application* IE::CreateApplication()
{
	return new TestGame();
}