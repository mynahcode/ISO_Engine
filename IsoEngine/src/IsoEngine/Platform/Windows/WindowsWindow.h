#pragma once

#include "IsoEngine/Window.h"
#include <GLFW/glfw3.h>

namespace IE
{
	class WindowsWindow : public Window
	{
		/* Public Functions and Methods*/
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		//Window Attributes
		inline void SetEventCallback(const EventCallback& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() override;

		/* Private Functions and Methods */
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		/* Private Variables */
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
