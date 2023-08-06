#include "iepch.h"
#include "IsoEngine/Core/Input.h"

#include "IsoEngine/Core/Application.h"
#include <GLFW/glfw3.h>

namespace IE
{
	bool Input::IsKeyPressed(KeyCode key)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<uint32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<uint32_t>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}


	/* For accessing values of the pair, can do pair.first, pair.second */
	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePos(); // love C++17

		return (float)x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePos();

		return (float)y;
	}
}