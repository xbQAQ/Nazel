#pragma once
#include <Nazel.h>

namespace Nazel{
class EditorLayer : public Layer{

public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(TimeStep deltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event& event) override;
private:
	Ref<Shader> m_FlatColorShader;
	Ref<VertexArray> m_SquareVA;
	Ref<FrameBuffer> m_Framebuffer;
	Ref<Texture2D> m_CheckerboardTexture;
	OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
}

