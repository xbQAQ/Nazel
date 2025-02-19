#include "pch.h"
#include "Application.h"
#include "Input.h"

#include "Nazel/RenderAPI/Renderer.h"

namespace Nazel {

#define BIND_EVENT_FUNCTION(X) std::bind(&Application::X, this, std::placeholders::_1)
Application* Application::s_Instance = nullptr;


Application::Application() {
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));
	s_Instance = this;

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

	// 1. 创建顶点数组
	m_VertexArray.reset(VertexArray::Create());

	// 使用OpenGL函数渲染一个三角形

	// 2. 把我们的CPU的顶点数据复制到GPU顶点缓冲中，供OpenGL使用
	// 顶点数据
	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
	// 4. 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局

	BufferLayout layout = {
		{ShaderDataType::Float3, "a_Position"},
		{ShaderDataType::Float4, "a_Color" }
	};
	m_VertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	// 3. 复制我们的CPU的索引数据到GPU索引缓冲中，供OpenGL使用
	uint32_t indices[3] = { 0, 1, 2 }; // 索引数据
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	m_SquareVA.reset(VertexArray::Create());

	float squareVertices[3 * 4] = {
		-0.75f, -0.75f, 0.0f,
		 0.75f, -0.75f, 0.0f,
		 0.75f,  0.75f, 0.0f,
		-0.75f,  0.75f, 0.0f
	};
	std::shared_ptr<VertexBuffer> squareVB;
	squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" }
						});
	m_SquareVA->AddVertexBuffer(squareVB);
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<IndexBuffer> squareIB;
	squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

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

	std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";
	std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";
	m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
}

Application::~Application() {
}

void Application::Run() {
	while (m_Running) {
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		
		Renderer::BeginScene();

		m_BlueShader->Bind();
		Renderer::Submit(m_SquareVA);

		m_Shader->Bind();
		Renderer::Submit(m_VertexArray);

		Renderer::EndScene();

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

