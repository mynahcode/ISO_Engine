#pragma once

#include "IsoEngine/Events/Event.h"

#include <sstream>

namespace IE
{
	class IsoEngine_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class IsoEngine_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool isRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << ", for " << m_IsRepeat << " repeats.";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int keycode;
		bool  m_IsRepeat;

	};

	class IsoEngine_API KeyReleasedEvent : public KeyEvent
	{
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {} 

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

	private:
		int keycode;
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
