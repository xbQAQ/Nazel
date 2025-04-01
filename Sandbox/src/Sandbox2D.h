#pragma once
#include <Nazel.h>

class Sandbox2D : public Nazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Nazel::TimeStep deltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Nazel::Event& event) override;
private:
	Nazel::OrthographicCameraController m_CameraController;
	Nazel::Ref<Nazel::VertexArray> m_SquareVA;
	Nazel::Ref<Nazel::Shader> m_FlatColorShader;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};

