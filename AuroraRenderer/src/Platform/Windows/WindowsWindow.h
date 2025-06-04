#pragma once

#include <windows.h>

#include "Aurora/Core/Window.h"


namespace Aurora
{
	class WindowsWindow : public Window
	{

	public:
		WindowsWindow(const WindowProperty& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		inline unsigned int GetWidth() const override { return width; };
		inline unsigned int GetHeight() const override { return height; };
		inline std::string GetName() const override { return title; };
		inline bool GetFullScreen() const override { return fullscreen; };

		void SetVSync(bool enabled) override;
		inline bool IsVSync() const override { return vSync; };

		void  SetEventCallback(const EventCallback& callback);

		inline virtual void* GetNativeWindow() const { return hwnd; }

		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	private: 
		void Init(const WindowProperty& property);
		void Shutdown();

		LPCTSTR windowsName;
		HINSTANCE hinstance;
		HWND hwnd;

		MSG msg;

		std::string title;
		unsigned int width, height;
		unsigned int posX, posY;
		bool vSync;
		bool fullscreen;

		EventCallback eventCallback;
	

	};

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


	static Aurora::WindowsWindow* windowsWindow = 0;
}

