#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

#include "Application.h"
#include "Nazel/Events/ApplicationEvent.h"
#include "Nazel/LayerStack.h"
#include "Nazel/imgui/ImguiLayer.h"

namespace Nazel {
class Application
{
public:
	Application();
	virtual ~Application();
	void Run();
	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	inline Window& GetWindow() { return *m_Window; }
	inline static Application& Get() { return *s_Instance; }
private:
	bool OnWindowClose(WindowCloseEvent& e); 
	bool OnWindowResize(WindowResizeEvent& e);
private:
	std::shared_ptr<Nazel::Window> m_Window;

	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	bool m_Minimized = false;
	LayerStack m_LayerStack;
	float m_LastFrameTime = 0.0f;
	static Application* s_Instance;
};

// 定义在Client中
Application* CreateApplication();
}

