#pragma once

#include "IsoMacros.h"
#include "IsoEngine/Events/Event.h"

namespace IE
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& window_title = "IsoEngine Application",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(window_title), Width(width), Height(height)
		{
		}
	};

	/* Abstract Interface Representing a desktop system based Window */
	class IsoEngine_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}