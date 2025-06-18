#pragma once

#include <windows.h>

#include "Aurora/Core/Window.h"
#include "Platform/Renderer/DirectX/DirectXContext.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace Aurora
{

	class WindowsWindow : public Window
	{

	public:
		WindowsWindow(const WindowProperty& props);
		virtual ~WindowsWindow();

		virtual void SetWindowVisibility(bool visibility) override;
		virtual void SetCursorVisibility(bool visibility) override;

		virtual void OnUpdate() override;
		virtual void OnNewFrame() override;

		inline unsigned int GetWidth() const override { return width; };
		inline unsigned int GetHeight() const override { return  height; };
		inline std::string GetName() const override { return  title; };

		void SetFullScreen(bool fullscreen) override;
		inline bool GetFullScreen() const override { return  fullscreen; };

		void SetVSync(bool enabled) override;
		inline bool IsVSync() const override { return  vSync; };

		void SetWindowMode(WindowMode mode);
	
		void SetResolutionAndRefreshRate(DisplayMode displayMode);

		void  SetEventCallback(const EventCallback& callback);

		DisplayMode GetCurrentDisplayMode() const;
	
		inline virtual void* GetNativeWindow() const { return hwnd; }
		void SetNativeWindow(HWND hwnd);

		std::vector<std::pair<unsigned int, unsigned int>> GetResolutions() const override;
		std::vector<std::pair<unsigned int, unsigned int>> GetRefreshRatesForResolution(unsigned int width, unsigned int height) const override;

		inline DirectXContext* GetContext() { return context; }

		LRESULT CALLBACK MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private: 
		void Init(const WindowProperty& property);
		void Shutdown();

		WindowMode windowMode = WindowMode::Windowed;

		LPCTSTR windowsName;
		HINSTANCE hinstance;
		HWND hwnd;

		MSG msg;

		std::vector<DisplayMode> displayModes;
	
		std::string title;
		unsigned int width, height;
		unsigned int posX, posY;
		bool vSync = true;
		bool fullscreen;

		EventCallback eventCallback;
		
		DirectXContext* context;
	};

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
