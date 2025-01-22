#include "pch.h"
#include "Application.h"
#include <GLFW/glfw3.h>

#define BIND_EVENT_FUNCTION(X) std::bind(&Application::X, this, std::placeholders::_1)

namespace Nazel {
Application::Application() {
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));
}

Application::~Application() {
}

void Application::Run() {
	while (m_Running) {
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e) {
	EventDispatcher dispatcher(e);
	switch (e.GetEventType()) {
	case EventType::WindowClose:
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));
		break;
	default:
		break;
	}

	LOG_CORE_TRACE("{0}", e.ToString());
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
	m_Running = false;
	return true;
}
}

