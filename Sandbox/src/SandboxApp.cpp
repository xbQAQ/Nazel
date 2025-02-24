#include <Nazel.h>
#include "imgui/imgui.h"
#include "Nazel/RenderAPI/Shader.h"
#include "Nazel/RenderAPI/Buffer.h"
#include "Nazel/RenderAPI/VertexArray.h"
#include "Nazel/RenderAPI/OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"

class ExampleLayer : public Nazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {
		// 1. 创建顶点数组
		m_VertexArray.reset(Nazel::VertexArray::Create());

		// 使用OpenGL函数渲染一个三角形

		// 2. 把我们的CPU的顶点数据复制到GPU顶点缓冲中，供OpenGL使用
		// 顶点数据
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		std::shared_ptr<Nazel::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Nazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		// 4. 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局

		Nazel::BufferLayout layout = {
			{Nazel::ShaderDataType::Float3, "a_Position"},
			{Nazel::ShaderDataType::Float4, "a_Color" }
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// 3. 复制我们的CPU的索引数据到GPU索引缓冲中，供OpenGL使用
		uint32_t indices[3] = { 0, 1, 2 }; // 索引数据
		std::shared_ptr<Nazel::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Nazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_SquareVA.reset(Nazel::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		std::shared_ptr<Nazel::VertexBuffer> squareVB;
		squareVB.reset(Nazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Nazel::ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Nazel::IndexBuffer> squareIB;
		squareIB.reset(Nazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// 着色器代码
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ProjectionView;
			uniform mat4 u_ModelTransform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;	
				gl_Position = u_ProjectionView * u_ModelTransform * vec4(a_Position, 1.0);	
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

		m_Shader.reset(new Nazel::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ProjectionView;
			uniform mat4 u_ModelTransform;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ProjectionView * u_ModelTransform * vec4(a_Position, 1.0);	
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
		m_BlueShader.reset(new Nazel::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}
	void OnUpdate(Nazel::TimeStep ts) override {
		LOG_EDITOR_INFO("timestep: {0}s, {1}ms", ts.GetSeconds(), ts.GetMilliseconds());
		if (Nazel::Input::IsKeyPressed(NZ_KEY_LEFT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		else if (Nazel::Input::IsKeyPressed(NZ_KEY_RIGHT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		if (Nazel::Input::IsKeyPressed(NZ_KEY_UP))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Nazel::Input::IsKeyPressed(NZ_KEY_DOWN))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Nazel::Input::IsKeyPressed(NZ_KEY_A))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		if (Nazel::Input::IsKeyPressed(NZ_KEY_D))
			m_CameraRotation += m_CameraRotationSpeed * ts;

		Nazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Nazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Nazel::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Nazel::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
			}
		}
		Nazel::Renderer::Submit(m_Shader, m_VertexArray);

		Nazel::Renderer::EndScene();
	}
	virtual void OnImGuiRender() override {
		//LOG_EDITOR_TRACE("Hello");
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}
	void OnEvent(Nazel::Event& event) override {
		if (event.GetEventType() == Nazel::EventType::KeyPressed) {
			Nazel::KeyPressedEvent& e = (Nazel::KeyPressedEvent&)event;
			if (e.GetKeyCode() == NZ_KEY_TAB)
				LOG_EDITOR_TRACE("Tab key is pressed (event)!");
			LOG_EDITOR_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
private:
	std::shared_ptr<Nazel::VertexArray> m_VertexArray;
	std::shared_ptr<Nazel::Shader> m_Shader;
	std::shared_ptr<Nazel::Shader> m_BlueShader;
	std::shared_ptr<Nazel::VertexArray> m_SquareVA;

	Nazel::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 60.0f;
};

class Sandbox : public Nazel::Application
{
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

Nazel::Application* Nazel::CreateApplication()
{
	return new Sandbox();
}


