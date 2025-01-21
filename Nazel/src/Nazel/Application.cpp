#include "pch.h"
#include "Application.h"
#include "Nazel/Events/ApplicationEvent.h"
#include "Nazel/Log.h"

namespace Nazel {
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication)) {
			LOG_EDITOR_TRACE(e.ToString());
		}
		if (e.IsInCategory(EventCategoryInput)) {
			LOG_EDITOR_TRACE(e.ToString());
		}
		while (true)
		{

		}
	}
}

