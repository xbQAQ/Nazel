#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

#include "Application.h"
#include "Nazel/Events/ApplicationEvent.h"
#include "Nazel/LayerStack.h"
#include "Nazel/imgui/ImguiLayer.h"

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

	inline Window& GetWindow() { return *m_Window; }
	inline static Application& Get() { return *s_Instance; }
private:
	bool OnWindowClose(WindowCloseEvent& e); 
private:
	uint32_t m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	LayerStack m_LayerStack;
	static Application* s_Instance;
};

// 定义在Client中
Application* CreateApplication();
}

