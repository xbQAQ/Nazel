#pragma once
#include "Nazel/Layer.h"
#include "Nazel/Events/KeyEvent.h"
#include "Nazel/Events/MouseEvent.h"
#include "Nazel/Events/ApplicationEvent.h"

namespace Nazel {
class NAZEL_API ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();
	void OnAttch() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Event& event) override;
private:
	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	bool OnMouseMovedEvent(MouseMovedEvent& e);
	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	bool OnKeyPressedEvent(KeyPressedEvent& e);
	bool OnKeyReleasedEvent(KeyReleasedEvent& e);
	bool OnKeyTypedEvent(KeyTypedEvent& e);
	bool OnWindowsResizeEvent(WindowResizeEvent& e);

private:
	float m_Time = 0.0f;
};
}