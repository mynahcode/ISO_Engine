#include <IsoEngine.h>

#include "Application.h"

#include "IsoEngine/Events/ApplicationEvent.h"
#include "IsoEngine/IsoLogger/IsoLogger.h"
#include "Events/Event.h"

namespace IE {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent ev(1280, 720);
		if (ev.IsInCategory(EventCategoryApplication))
		{
			ISOLOGGER_TRACE("EventCategoryApplication executed: ", ev);
		}

		if (ev.IsInCategory(EventCategoryInput))
		{
			ISOLOGGER_TRACE("EventCategoryInput executed: ", ev);
		}

		if (ev.IsInCategory(EventCategoryKeyboard))
		{
			ISOLOGGER_TRACE("EventCategoryKeyboard executed: ", ev);
		}

		if (ev.IsInCategory(EventCategoryMouse))
		{
			ISOLOGGER_TRACE("EventCategoryMouse executed: ", ev);
		}

		if (ev.IsInCategory(EventCategoryMouseButton))
		{
			ISOLOGGER_TRACE("EventCategoryMouseButton executed: ", ev);
		}

		while (true);
	}
}