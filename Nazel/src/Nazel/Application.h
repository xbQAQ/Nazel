#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

#include "Application.h"
#include "Nazel/Events/ApplicationEvent.h"

namespace Nazel {
class NAZEL_API Application
{
public:
	Application();
	virtual ~Application();
	void Run();
	void OnEvent(Event& e);
private:
	bool OnWindowClose(WindowCloseEvent& e); 
private:
	std::unique_ptr<Window> m_Window;
	bool m_Running = true;
};

// 定义在Client中
Application* CreateApplication();
}

