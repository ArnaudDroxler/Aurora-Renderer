#pragma once

#include <windows.h>

#include "Aurora/Core/Window.h"
#include "Platform/Renderer/DirectX/DirectXContext.h"

namespace Aurora
{
	class WindowsWindow : public Window
	{

	public:
		WindowsWindow(const WindowProperty& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		inline unsigned int GetWidth() const override { return width; };
		inline unsigned int GetHeight() const override { return  height; };
		inline std::string GetName() const override { return  title; };
		inline bool GetFullScreen() const override { return  fullscreen; };

		void SetVSync(bool enabled) override;
		inline bool IsVSync() const override { return  vSync; };

		void  SetEventCallback(const EventCallback& callback);

		inline virtual void* GetNativeWindow() const { return hwnd; }
		void SetNativeWindow(HWND hwnd);

		LRESULT CALLBACK MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam);

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
		
		DirectXContext* context;
	};

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

}

