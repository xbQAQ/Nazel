#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

#include "Application.h"
#include "Nazel/Events/ApplicationEvent.h"
#include "Nazel/LayerStack.h"
#include "Nazel/imgui/ImguiLayer.h"
#include "RenderAPI/Shader.h"
#include "RenderAPI/Buffer.h"
#include "RenderAPI/VertexArray.h"

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
	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Shader> m_BlueShader;
	std::shared_ptr<VertexArray> m_SquareVA;
	std::shared_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	LayerStack m_LayerStack;
	static Application* s_Instance;
};

// 定义在Client中
Application* CreateApplication();
}

