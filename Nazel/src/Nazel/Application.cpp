#include "pch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>

namespace Nazel {

#define BIND_EVENT_FUNCTION(X) std::bind(&Application::X, this, std::placeholders::_1)
Application* Application::s_Instance = nullptr;

Application::Application() {
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));
	s_Instance = this;

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

	// 使用OpenGL函数渲染一个三角形
	// 顶点数据
	float vertices[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	unsigned int indices[3] = { 0, 1, 2 }; // 索引数据
	// 0.生成顶点数组对象VAO、顶点缓冲对象VBO、索引缓冲对象EBO
	glGenVertexArrays(1, &m_VertexArray);
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	// 1. 绑定顶点数组对象
	glBindVertexArray(m_VertexArray);
	// 2. 把我们的CPU的顶点数据复制到GPU顶点缓冲中，供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. 复制我们的CPU的索引数据到GPU索引缓冲中，供OpenGL使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局
	glEnableVertexAttribArray(0);// 开启glsl的layout = 0输入
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	// 着色器代码
	std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";
	std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";
	
	m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
}

Application::~Application() {
	glDeleteProgram(m_RendererID);
}

void Application::Run() {
	while (m_Running) {
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->Bind();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();

		auto [x, y] = Input::GetMousePosition();
		//LOG_CORE_TRACE("({0}, {1})", x, y);

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

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

	// LOG_CORE_TRACE("{0}", e.ToString());
}

void Application::PushLayer(Layer* layer) {
	m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer) {
	m_LayerStack.PushOverlay(layer);
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
	m_Running = false;
	return true;
}
}

