#pragma once
#include "Nazel/Layer.h"

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
	float m_Time = 0.0f;
};
}