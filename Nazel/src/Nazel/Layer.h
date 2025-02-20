#pragma once

#include "Nazel/Core.h"
#include "Nazel/Events/Event.h"
#include "Core/TimeStep.h"

namespace Nazel {
class NAZEL_API Layer
{
public:
	Layer(const std::string& debugName = "Layer");
	virtual ~Layer();

	virtual void OnAttach() { }
	virtual void OnDetach() { }
	virtual void OnUpdate(TimeStep deltaTime) { }
	virtual void OnImGuiRender() { }
	virtual void OnEvent(Event& event) { }

	inline const std::string& GetName() const { return m_DebugName; }
private:
	std::string m_DebugName;
};
}

