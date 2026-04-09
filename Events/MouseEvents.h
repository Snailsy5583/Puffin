#pragma once

#include "../Window.h"
#include "Events.h"

namespace Engine
{
	class MouseButtonEvent : public Event
	{
	public:
		void GetMousePos(float &outX, float &outY) const
		{
			outX = m_MousePosX;
			outY = m_MousePosY;
		}

		void GetMousePixelPos(int &x, int &y) const
		{
			auto winSize = m_Window.GetWindowSize();
			x = (int) ((float) winSize.x * m_MousePosX);
			y = (int) ((float) winSize.y * m_MousePosY);
		}

		int GetMouseButton() const { return m_Button; }

	protected:
		MouseButtonEvent(const Window &window,
						 int button,
						 float mouseX,
						 float mouseY)
			: Event(window), m_Button(button), m_MousePosX(mouseX),
			  m_MousePosY(mouseY)
		{
		}

		int m_Button;
		float m_MousePosX, m_MousePosY;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const Window &window,
								int button,
								float mouseX,
								float mouseY)
			: MouseButtonEvent(window, button, mouseX, mouseY)
		{
		}

		static EventType GetStaticType() { return MouseButtonPressed; }

		EventType GetEventType() const override { return GetStaticType(); }

		const char *GetName() const override { return "MouseButtonPressed"; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const Window &window,
								 int button,
								 float mouseX,
								 float mouseY)
			: MouseButtonEvent(window, button, mouseX, mouseY)
		{
		}

		static EventType GetStaticType() { return MouseButtonReleased; }

		EventType GetEventType() const override { return GetStaticType(); }

		const char *GetName() const override { return "MouseButtonReleased"; }
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const Window &window, float mouseX, float mouseY)
			: Event(window), m_MousePos(mouseX, mouseY)
		{
		}

		~MouseMovedEvent() override { m_PrevMousePos = m_MousePos; }

		static EventType GetStaticType() { return MouseMoved; }

		EventType GetEventType() const override { return GetStaticType(); }

		const char *GetName() const override { return "MouseMoved"; }

		void GetMousePixelPos(int &x, int &y) const
		{
			auto winSize = m_Window.GetWindowSize();
			x = (int) ((float) winSize.x * m_MousePos.x);
			y = (int) ((float) winSize.y * m_MousePos.y);
		}

		void GetMousePosition(float &outX, float &outY) const
		{
			outX = m_MousePos.x;
			outY = m_MousePos.y;
		}

		glm::vec2 GetMousePosition() const { return m_MousePos; }
		glm::vec2 GetDelta() const { return m_MousePos - m_PrevMousePos; }

	private:
		glm::vec2 m_MousePos;
		inline static glm::vec2 m_PrevMousePos;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const Window &window, float x, float y)
			: Event(window), m_Delta(x, y)
		{
		}

		static EventType GetStaticType() { return EventType::MouseScrolled; }

		EventType GetEventType() const override { return GetStaticType(); }

		const char *GetName() const override { return "MouseScrolled"; }

		glm::vec2 GetScrollAmount() const { return m_Delta; }

	private:
		glm::vec2 m_Delta {};
	};

}	 // namespace Engine