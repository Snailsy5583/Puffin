#pragma once

#include "Events.h"

namespace Engine
{


	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent(const Window &window) : Event(window) { }

		static EventType GetStaticType() { return WindowClosed; }

		EventType GetEventType() const override { return GetStaticType(); }

		const char *GetName() const override { return "WindowClosed"; }
	};

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(const Window &window, int width, int height)
			: Event(window), m_Width(width), m_Height(height)
		{
		}

		static EventType GetStaticType() { return WindowResized; }

		EventType GetEventType() const override { return GetStaticType(); }

		const char *GetName() const override { return "WindowResized"; }

		[[nodiscard]] int GetWidth() const { return m_Width; }

		[[nodiscard]] int GetHeight() const { return m_Height; }

	private:
		int m_Width, m_Height;
	};


}	 // namespace Engine