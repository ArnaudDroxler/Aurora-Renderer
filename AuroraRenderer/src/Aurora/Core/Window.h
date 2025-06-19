#pragma once

#include "aurorapch.h"
#include "Aurora/Events/Event.h"
#include "Platform/Renderer/GraphicContext.h"

namespace Aurora
{

	enum class WindowMode {
		Windowed,
		BorderlessFullscreen,
		ExclusiveFullscreen
	};

	struct WindowProperty
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowMode windowMode;

		WindowProperty(const std::string& title = "Aurora Render", unsigned int width = 1920, unsigned int height = 1080, WindowMode windowMode = WindowMode::Windowed) : Title(title), Width(width), Height(height), windowMode(windowMode)
		{

		}

	};

	struct DisplayMode
	{
		unsigned int width;
		unsigned int height;
		unsigned int refreshRateNumerator;
		unsigned int refreshRateDenominator;

		float GetRefreshRate() const
		{
			return static_cast<float>(refreshRateNumerator) / refreshRateDenominator;
		}

		std::string ToString() const
		{
			return std::to_string(width) + "x" + std::to_string(height) + " @ " + std::to_string(GetRefreshRate()) + "Hz";
		}
	};

	class  Window
	{
	public:

		virtual ~Window() = default;

		using EventCallback = std::function<void(Event&)>;

		virtual void OnUpdate() = 0;
		virtual void OnNewFrame() = 0;

		virtual void SetWindowVisibility(bool visibility) = 0;
		virtual void SetCursorVisibility(bool visibility) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual std::string GetName() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool GetVSync() const = 0;

		virtual void SetWindowMode(WindowMode mode) = 0;
		virtual WindowMode GetWindowMode() const = 0;

		virtual void SetResolutionAndRefreshRate(DisplayMode displayMode) = 0;

		virtual DisplayMode GetCurrentDisplayMode() const = 0;

		virtual std::vector<std::pair<unsigned int, unsigned int>> GetResolutions() const = 0;
		virtual std::vector< std::pair<unsigned int, unsigned int>> GetRefreshRatesForResolution(unsigned int width, unsigned int height) const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SetEventCallback(const EventCallback& callback) = 0;

		static Window* Create(const WindowProperty& props = WindowProperty());

	};
	
}