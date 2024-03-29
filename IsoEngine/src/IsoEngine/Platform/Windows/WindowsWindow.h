#pragma once

#include "IsoEngine/Core/Window.h"
#include "IsoEngine/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace IE
{
	class WindowsWindow : public Window
	{
		/* Public Functions and Methods*/
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return m_Window; }

		/* Private Functions and Methods */
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		/* Private Variables */ 
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

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

