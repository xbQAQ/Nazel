#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280 / 720) {
}

void Sandbox2D::OnAttach() {
	m_SquareVA = Nazel::VertexArray::Create();
	float squareVertices[3 * 4] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f,
	};
	Nazel::Ref<Nazel::VertexBuffer> squareVB;
	squareVB = Nazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ Nazel::ShaderDataType::Float3, "a_Position" }
						});
	m_SquareVA->AddVertexBuffer(squareVB);
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Nazel::Ref<Nazel::IndexBuffer> squareIB;
	squareIB = Nazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Nazel::Shader::Create("assets/shaders/flatColor.glsl");
}

void Sandbox2D::OnDetach() {
}

void Sandbox2D::OnUpdate(Nazel::TimeStep deltaTime) {
	m_CameraController.OnUpdate(deltaTime);

	Nazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Nazel::RenderCommand::Clear();

	std::dynamic_pointer_cast<Nazel::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Nazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Nazel::Renderer::BeginScene(m_CameraController.GetCamera());
	Nazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	Nazel::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Nazel::Event& event) {
	m_CameraController.OnEvent(event);
}
