#include <Nazel.h>
#include "imgui/imgui.h"

class ExampleLayer : public Nazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {
	}
	void OnUpdate() override {
		if (Nazel::Input::IsKeyPressed(NZ_KEY_TAB))
			LOG_EDITOR_TRACE("Tab key is pressed (poll)!");
	}
	virtual void OnImGuiRender() override {
		LOG_EDITOR_TRACE("Hello");
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


