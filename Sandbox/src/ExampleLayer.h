#pragma once
#include <Nazel.h>

class ExampleLayer : public Nazel::Layer
{
public:
	ExampleLayer();
	void OnUpdate(Nazel::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Nazel::Event& event) override;
private:
	Nazel::ShaderLibrary m_ShaderLibrary;
	Nazel::Ref<Nazel::VertexArray> m_VertexArray;
	Nazel::Ref<Nazel::Shader> m_Shader;
	Nazel::Ref<Nazel::Shader> m_FlatColorShader, m_TextureShader;
	Nazel::Ref<Nazel::VertexArray> m_SquareVA;
	Nazel::Ref<Nazel::Texture2D> m_Texture, m_ChernoLogo;

	Nazel::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

