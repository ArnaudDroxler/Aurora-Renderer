
#pragma once

#include "aurorapch.h"

#include "Event.h"

namespace Aurora
{
	class  KeyEvent : public  Event
	{
	public:

		inline int GetKeyCode() { return keyCode; }

		virtual int GetCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; }

	protected:

		KeyEvent(int keyCode) : keyCode(keyCode) {}

		int keyCode;
	};

	class  KeyPressedEvent : public KeyEvent
	{
	public:

		KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), repeatCount(repeatCount) {}

		static EventType GetStaticType() { return EventType::KeyPressed; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyPressed"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
			return ss.str();
		}

	private:
		int repeatCount;
	};

	class  KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		static EventType GetStaticType() { return EventType::KeyReleased; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyReleased"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keyCode;
			return ss.str();
		}
	private:
	};

	class  KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

		static EventType GetStaticType() { return EventType::KeyTyped; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyTyped"; }

		inline int GetKeyCode() { return keyCode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << keyCode;
			return ss.str();
		}
	};
}
