#include "pch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Nazel {

#define BIND_EVENT_FUNCTION(X) std::bind(&Application::X, this, std::placeholders::_1)
Application* Application::s_Instance = nullptr;

Application::Application() {
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));
	s_Instance = this;
}

Application::~Application() {
}

void Application::Run() {
	while (m_Running) {
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();

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
	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
		(*--it)->OnEvent(e);
		if (e.bIsHandled)
			break;
	}

	LOG_CORE_TRACE("{0}", e.ToString());
}

void Application::PushLayer(Layer* layer) {
	m_LayerStack.PushLayer(layer);
	layer->OnAttch();
}

void Application::PushOverlay(Layer* layer) {
	m_LayerStack.PushOverlay(layer);
	layer->OnAttch();
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
	m_Running = false;
	return true;
}
}

