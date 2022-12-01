#pragma once

#include "pfpch.h"

#include "Puffin/Core.h"

namespace Puffin
{

	enum class EventType
	{
		None=0,

		WindowClose, WindowResize, WindowFocus, WindowLostFocus,
		AppTick, AppRender,
		KeyPressed, KeyReleased, KeyRepeat,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None=0,

		EventCategoryWindow					 = BIT(0),
		EventCategoryApplication						 = BIT(1),
		EventCategoryInput					 = BIT(2),
			EventCategoryKeyboard			 = BIT(3),
			EventCategoryMouse				 = BIT(4),
				EventCategoryMouseButton		 = BIT(5),
	};

#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategories() const { return category; }

	class PUFFIN_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategories() const = 0;
		virtual std::string ToString() const { return GetName(); }

		virtual bool IsInCategory(EventCategory category)
		{
			return GetCategories() & category;
		}

		inline bool GetHandled() const { return m_Handled; }
		inline void SetHandled(bool value) { m_Handled = value; }
	protected:
		bool m_Handled = false;
	};

	class PUFFIN_API EventDispatcher
	{
		template <typename T>
		using EventFunc = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{}

		template <typename T>
		bool Dispatch(EventFunc<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}