#pragma onece

#include "Event.h"
#include <sstream>

namespace Nazel {

class NAZEL_API KeyEvent : public Event
{
public:
	inline int GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard || EventCategoryInput)
protected:
	KeyEvent(int keyCode) : m_KeyCode(keyCode) { }
	int m_KeyCode;
};

class NAZEL_API KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) { }

	inline int GetRepeatCount() const { return m_RepeatCount; }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)

private:
	int m_RepeatCount;
};

class NAZEL_API KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keycode, int repeatCount) : KeyEvent(keycode) { }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

}