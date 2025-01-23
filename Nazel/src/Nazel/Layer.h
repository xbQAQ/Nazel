#pragma once

#include "Nazel/Core.h"
#include "Nazel/Events/Event.h"

namespace Nazel {
class NAZEL_API Layer
{
public:
	Layer(const std::string& debugName = "Layer");
	virtual ~Layer();

	virtual void OnAttch() { }
	virtual void OnDetach() { }
	virtual void OnUpdate() { }
	virtual void OnEvent(Event& event) { }

	inline const std::string& GetName() const { return m_DebugName; }
private:
	std::string m_DebugName;
};
}

