#pragma once

#include "Event.h"

namespace Puffin
{

	class PUFFIN_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y)
		{}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoved: (" << m_MouseX << ", " << m_MouseY << ")" << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse)
		

	private:
		float m_MouseX, m_MouseY;
	};

	class PUFFIN_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float amount)
			: m_Amount(amount)
		{}

		inline float GetScrollAmount() const { return m_Amount; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolled: " << m_Amount << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse)
	private:
		float m_Amount;
	};

	class PUFFIN_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse | EventCategory::EventCategoryMouseButton)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button)
		{}

		int m_Button;
	};

	class PUFFIN_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed: " << m_Button << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class PUFFIN_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleased: " << m_Button << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}