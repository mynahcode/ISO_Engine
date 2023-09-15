#pragma once

#include "IsoMacros.h"
#include "IsoEngine/Events/Event.h"

namespace IE
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& window_title = "IsoEngine",
			uint32_t width = 1920,
			uint32_t height = 1080)
			: Title(window_title), Width(width), Height(height)
		{
		}
	};

	/* Abstract Interface Representing a desktop system based Window */
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		//Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}