#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {
}

void Sandbox2D::OnAttach() {
	PROFILE_FUNCTION();

	m_CheckerboardTexture = Nazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() {
	PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Nazel::TimeStep deltaTime) {
	PROFILE_FUNCTION();

	m_CameraController.OnUpdate(deltaTime);
	Nazel::Renderer2D::ResetStats();
	{
		PROFILE_SCOPE("Renderer Prep");
		Nazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Nazel::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += deltaTime * 50.0f;

		PROFILE_SCOPE("Renderer Draw");
		Nazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Nazel::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Nazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Nazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		Nazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 100.0f, 100.0f }, m_CheckerboardTexture, 10.0f);
		Nazel::Renderer2D::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 20.0f);
		Nazel::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);

		Nazel::Renderer2D::EndScene();

		Nazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Nazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Nazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");

	auto stats = Nazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.Drawcalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	for (auto& result : m_ProfileResults) {
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();
	ImGui::End();
}

void Sandbox2D::OnEvent(Nazel::Event& event) {
	m_CameraController.OnEvent(event);
}
