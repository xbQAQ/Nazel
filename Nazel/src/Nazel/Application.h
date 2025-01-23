#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

#include "Application.h"
#include "Nazel/Events/ApplicationEvent.h"
#include "Nazel/LayerStack.h"

namespace Nazel {
class NAZEL_API Application
{
public:
	Application();
	virtual ~Application();
	void Run();
	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);
private:
	bool OnWindowClose(WindowCloseEvent& e); 
private:
	std::unique_ptr<Window> m_Window;
	bool m_Running = true;
	LayerStack m_LayerStack;
};

// 定义在Client中
Application* CreateApplication();
}

