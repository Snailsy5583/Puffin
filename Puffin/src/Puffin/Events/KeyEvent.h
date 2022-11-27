#pragma once

#include "Event.h"

namespace Puffin
{

	class PUFFIN_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard)

	protected:
		KeyEvent(int KeyCode)
			: m_KeyCode(KeyCode) {}

		int m_KeyCode;
	};

	class PUFFIN_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int KeyCode, bool IsRepeat)
			: KeyEvent(KeyCode), m_IsRepeat(IsRepeat)
		{}

		EVENT_CLASS_TYPE(KeyPressed);

		inline bool GetIsRepeat() const { return m_IsRepeat; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key " << m_KeyCode << (m_IsRepeat ? " is repeated." : " is pressed.") << std::endl;
			return ss.str();
		}
	protected:
		bool m_IsRepeat;
	};

	class PUFFIN_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int KeyCode)
			: KeyEvent(KeyCode)
		{}

		EVENT_CLASS_TYPE(KeyReleased);

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key " << m_KeyCode << " is released.";
			return ss.str();
		}
	};

}