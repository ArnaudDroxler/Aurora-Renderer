#pragma once

#include "aurorapch.h"
#include "Aurora/Events/Event.h"
#include "Platform/Renderer/GraphicContext.h"

namespace Aurora
{

	struct WindowProperty
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool Fullscreen;

		WindowProperty(const std::string& title = "Aurora Render", unsigned int width = 1280, unsigned int height = 720, bool fullscreen = false) : Title(title), Width(width), Height(height), Fullscreen(fullscreen)
		{
		}

	};

	class  Window
	{
	public:

		virtual ~Window() = default;

		using EventCallback = std::function<void(Event&)>;

		virtual void OnUpdate() = 0;
		virtual void OnNewFrame() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual std::string GetName() const = 0;
		virtual bool GetFullScreen() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SetEventCallback(const EventCallback& callback) = 0;

		static Window* Create(const WindowProperty& props = WindowProperty());

	};
	
}