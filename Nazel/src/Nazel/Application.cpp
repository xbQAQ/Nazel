#include "pch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>

namespace Nazel {

#define BIND_EVENT_FUNCTION(X) std::bind(&Application::X, this, std::placeholders::_1)
Application* Application::s_Instance = nullptr;

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:   return GL_FLOAT;
	case ShaderDataType::Float2:  return GL_FLOAT;
	case ShaderDataType::Float3:  return GL_FLOAT;
	case ShaderDataType::Float4:  return GL_FLOAT;
	case ShaderDataType::Mat3:    return GL_FLOAT;
	case ShaderDataType::Mat4:    return GL_FLOAT;
	case ShaderDataType::Int:     return GL_INT;
	case ShaderDataType::Int2:    return GL_INT;
	case ShaderDataType::Int3:    return GL_INT;
	case ShaderDataType::Int4:    return GL_INT;
	case ShaderDataType::Bool:    return GL_BOOL;
	}
	NZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

Application::Application() {
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));
	s_Instance = this;

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

	// 0.生成顶点数组对象VAO、顶点缓冲对象VBO、索引缓冲对象EBO
	// 
	// 1. 绑定顶点数组对象
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	// 使用OpenGL函数渲染一个三角形
	
	// 2. 把我们的CPU的顶点数据复制到GPU顶点缓冲中，供OpenGL使用
	// 顶点数据
	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};
	m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

	// 3. 复制我们的CPU的索引数据到GPU索引缓冲中，供OpenGL使用
	uint32_t indices[3] = { 0, 1, 2 }; // 索引数据
	m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
	
	// 4. 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局

	{
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color" }
		};
		m_VertexBuffer->SetLayout(layout);
	}

	uint32_t index = 0;
	const auto& layout = m_VertexBuffer->GetLayout();
	for (const auto& element : layout) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 
							  element.GetComponentCount(), 
							  ShaderDataTypeToOpenGLBaseType(element.Type), 
							  element.Normalized, 
							  layout.GetStride(), 
							  (const void*)element.Offset);
		index++;
	}

	// 着色器代码
	std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;	
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";
	std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
}

Application::~Application() {
}

void Application::Run() {
	while (m_Running) {
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->Bind();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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

