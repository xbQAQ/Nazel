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
	Nazel::Ref<Nazel::Texture2D> m_CheckerboardTexture;
	Nazel::Ref<Nazel::FrameBuffer> m_Framebuffer;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;
};

