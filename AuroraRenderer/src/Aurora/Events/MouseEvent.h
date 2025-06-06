#pragma once

#include "aurorapch.h"

#include "Event.h"

namespace Aurora
{
	class  MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float mouseX, float mouseY) : mouseX(mouseX), mouseY(mouseY) {}

		inline float GetMouseX() { return mouseX; }
		inline float GetMouseY() { return mouseY; }

		static EventType GetStaticType() { return EventType::MouseMoved; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseMoved"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent:  (" << mouseX << "," << mouseY << ")";
			return ss.str();
		}

		int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

	private:
		float mouseX, mouseY;
	};

	class  MouseScrolledEvent : public Event
	{
	public:

		MouseScrolledEvent(float xOffset, float yOffset) : xOffset(xOffset), yOffset(yOffset) {}

		static EventType GetStaticType() { return EventType::MouseScrolled; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseScrolled"; }


		inline float GetX() { return xOffset; }
		inline float GetY() { return yOffset; }


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent:  (" << xOffset << "," << yOffset << ")";
			return ss.str();
		}

		int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

	private:
		float xOffset, yOffset;
	};

	class  MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return buttonId; }

		int GetCategoryFlags() const override { return EventCategoryMouseButton | EventCategoryInput; }

	protected:
		MouseButtonEvent(int buttonId) : buttonId(buttonId) {}

		int buttonId;
	};

	class  MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent (int buttonId, float mouseX, float mouseY) : MouseButtonEvent(buttonId), mouseX(mouseX), mouseY(mouseY) {}

		inline float GetMouseX() { return mouseX; }
		inline float GetMouseY() { return mouseY; }

		static EventType GetStaticType() { return EventType::MouseButtonPressed; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseButtonPressed"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << buttonId;
			return ss.str();
		}

	private:
		float mouseX, mouseY;
	};

	class  MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int buttonId) : MouseButtonEvent(buttonId) {}

		static EventType GetStaticType() { return EventType::MouseButtonReleased; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "MouseButtonReleased"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << buttonId;
			return ss.str();
		}
	};
}
#pragma once
