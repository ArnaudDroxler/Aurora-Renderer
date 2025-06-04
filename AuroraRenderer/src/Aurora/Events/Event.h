#pragma once

#include "aurorapch.h"

namespace Aurora
{

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = (1 << 0),
		EventCategoryInput = (1 << 1),
		EventCategoryKeyboard = (1 << 2),
		EventCategoryMouse = (1 << 3),
		EventCategoryMouseButton = (1 << 4)
	};

	class  Event
	{
	public:

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory eventCategory)
		{
			return GetCategoryFlags() & eventCategory;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : event(event) {}

		// F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (event.GetEventType() == T::GetStaticType())
			{
				event.Handled = func(static_cast<T&>(event));
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}