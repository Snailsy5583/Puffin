//
// Created by r6awe on 8/14/2025.
//

#ifndef PUFFINTEXTEDITOR_KEYBOARDEVENTS_H
#define PUFFINTEXTEDITOR_KEYBOARDEVENTS_H

#include "Events.h"

namespace Engine
{
	class KeyboardCharTypedEvent final : public Event
	{
	public:
		explicit KeyboardCharTypedEvent(const Window &window, unsigned int cp)
			: Event(window), m_Char((char) cp)
		{
		}

		[[nodiscard]] char GetChar() const { return m_Char; }

	public:
		static EventType GetStaticType()
		{ return KeyboardCharTyped; }
		EventType GetEventType() const override
		{ return GetStaticType(); }

		const char *GetName() const override { return "KeyboardCharTyped"; }

	private:
		char m_Char;
	};

	class KeyboardEvent : public Event
	{
	public:
		virtual int GetKey() const { return m_Key; }
		virtual int GetPhysicalKey() const { return m_PhysicalKey; }
		virtual int GetModifiers() const { return m_Modifiers; }

	protected:
		KeyboardEvent(const Window &window, int key, int scancode, int mods)
			: Event(window), m_Key(key), m_PhysicalKey(scancode),
			  m_Modifiers(mods)
		{
		}

	protected:
		int m_Key, m_PhysicalKey, m_Modifiers;
	};

	class KeyboardKeyPressedEvent final : public KeyboardEvent
	{
	public:
		KeyboardKeyPressedEvent(const Window &window,
								int key,
								int scancode,
								int mods)
			: KeyboardEvent(window, key, scancode, mods)
		{
		}

		static EventType GetStaticType()
		{ return KeyboardKeyPressed; }
		EventType GetEventType() const override
		{ return GetStaticType(); }

		const char *GetName() const override
		{ return "KeyboardKeyPressed"; }
	};

	class KeyboardKeyReleasedEvent final : public KeyboardEvent
	{
	public:
		KeyboardKeyReleasedEvent(const Window &window,
								 int key,
								 int scancode,
								 int mods)
			: KeyboardEvent(window, key, scancode, mods)
		{
		}

		static EventType GetStaticType()
		{ return KeyboardKeyReleased; }
		EventType GetEventType() const override
		{ return GetStaticType(); }

		const char *GetName() const override
		{ return "KeyboardKeyReleased"; }
	};

	class KeyboardKeyRepeatedEvent final : public KeyboardEvent
	{
	public:
		KeyboardKeyRepeatedEvent(const Window &window,
								 int key,
								 int scancode,
								 int mods)
			: KeyboardEvent(window, key, scancode, mods)
		{
		}

		static EventType GetStaticType()
		{ return KeyboardKeyRepeated; }
		EventType GetEventType() const override
		{ return GetStaticType(); }

		const char *GetName() const override
		{ return "KeyboardKeyRepeated"; }
	};


}	 // namespace Engine

#endif	  // PUFFINTEXTEDITOR_KEYBOARDEVENTS_H
