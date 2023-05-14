#pragma once

#include "IsoEngine/IsoMacros.h"
#include "IsoEngine/Window.h"

struct GLFWwindow;

namespace IE
{
	class IsoEngine_API WindowsWindow : public Window
	{
		/* Public Functions and Methods*/
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

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

