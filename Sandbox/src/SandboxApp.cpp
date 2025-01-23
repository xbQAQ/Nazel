#include <Nazel.h>

class ExampleLayer : public Nazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {
	}
	void OnUpdate() override {
		LOG_EDITOR_INFO("ExampleLayer::Update");
	}
	void OnEvent(Nazel::Event& event) override {
		LOG_EDITOR_INFO("{0}", event.ToString());
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


