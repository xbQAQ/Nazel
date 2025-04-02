#include "pch.h"
#include "Application.h"
#include "Input.h"

#include "Nazel/RenderAPI/Renderer.h"
#include "GLFW/glfw3.h"

namespace Nazel {

#define BIND_EVENT_FUNCTION(X) std::bind(&Application::X, this, std::placeholders::_1)
Application* Application::s_Instance = nullptr;


Application::Application() {
	PROFILE_FUNCTION();

	NZ_CORE_ASSERT(!s_Instance, "Application already exists!");
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));
	s_Instance = this;

	Renderer::Init();

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);
}

Application::~Application() {
	PROFILE_FUNCTION();

	Renderer::Shutdown();
}

void Application::Run() {
	PROFILE_FUNCTION();

	while (m_Running) {
		PROFILE_SCOPE("RunLoop");

		float time = (float)glfwGetTime();
		TimeStep deltaTime = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if (!m_Minimized) {
			{
				PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltaTime);
			}

			{
				PROFILE_SCOPE("LayerStack OnImGuiRender");

				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}
		}
		
		auto [x, y] = Input::GetMousePosition();
		//LOG_CORE_TRACE("({0}, {1})", x, y);
		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e) {
	PROFILE_FUNCTION();

	EventDispatcher dispatcher(e);
	switch (e.GetEventType()) {
	case EventType::WindowClose:
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));
		break;
	case EventType::WindowResize:
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OnWindowResize));
	default:
		break;
	}
	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
		(*--it)->OnEvent(e);
		if (e.bIsHandled)
			break;
	}

	// LOG_CORE_TRACE("{0}", e.ToString());
}

void Application::PushLayer(Layer* layer) {
	PROFILE_FUNCTION();

	m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer) {
	PROFILE_FUNCTION();

	m_LayerStack.PushOverlay(layer);
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
	PROFILE_FUNCTION();

	m_Running = false;
	return true;
}
bool Application::OnWindowResize(WindowResizeEvent& e) {
	PROFILE_FUNCTION();

	if (e.GetWidth() == 0 || e.GetHeight() == 0) {
		m_Minimized = true;
		return false;
	}
	m_Minimized = false;
	Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
	return false;
}
}

