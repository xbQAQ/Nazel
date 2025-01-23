#include "pch.h"
#include "ImguiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include "Nazel/Application.h"


namespace Nazel {
ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {
}

ImGuiLayer::~ImGuiLayer() {
}

void ImGuiLayer::OnAttch() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	//��������м�����ӳ�䣬�°��imgui�Ѿ�����Ҫ�ˣ������Ǿɰ�Ĳ�����ʽ
	//io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	//����ֱ���޸�imgui��ImGuiKey���޸�
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach() {
}

void ImGuiLayer::OnUpdate() {
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
	float time = (float)glfwGetTime();
	io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
	m_Time = time;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	static bool show = true;
	ImGui::ShowDemoWindow(&show);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& event) {
}
}

