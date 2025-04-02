#pragma once
#include "Nazel/Core/Layer.h"
#include "Nazel/Events/KeyEvent.h"
#include "Nazel/Events/MouseEvent.h"
#include "Nazel/Events/ApplicationEvent.h"

namespace Nazel {
class NAZEL_API ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void Begin();
	void End();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

private:
	float m_Time = 0.0f;
};
}