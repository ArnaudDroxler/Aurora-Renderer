#include "aurorapch.h"
#include "WindowsWindow.h"
#include "Aurora/Events/ApplicationEvent.h"


namespace Aurora
{


	Window* Window::Create(const WindowProperty& props)
	{
		return new WindowsWindow(props);
	}


	Aurora::WindowsWindow::WindowsWindow(const WindowProperty& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		vSync = enabled;
	}

	void WindowsWindow::SetEventCallback(const EventCallback& callback)
	{
		std::cout << "[DEBUG] SetEventCallback called" << std::endl;
		std::cout << "[DEBUG] Callback valid before assignment: " << (callback ? "YES" : "NO") << std::endl;
		eventCallback = callback;


		std::cout << "[DEBUG] eventCallback valid after assignment: " << (eventCallback ? "YES" : "NO") << std::endl;
	}

	LRESULT CALLBACK WindowsWindow::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		switch (umessage)
		{
			
			case WM_CLOSE:
			{
				Debug::CoreLog("WM_CLOSE");
				WindowCloseEvent event;
				eventCallback(event);
				return 0;
			}

			case WM_SIZE:
			{

				return 0;
			}
	
			default:
			{
				return DefWindowProc(hwnd, umessage, wparam, lparam);
			}
		}
	}

	void WindowsWindow::Init(const WindowProperty& props)
	{

		title = props.Title;
		height = props.Height;
		width = props.Width;
		fullscreen = props.Fullscreen;

		WNDCLASSEX wc = {};

		hinstance = GetModuleHandle(NULL);

		std::wstring titleWStr(title.begin(), title.end());
		windowsName = titleWStr.c_str();

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = Aurora::WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = windowsName;
		wc.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&wc);

		DEVMODE dmScreenSettings;

		int screenHeight = 0, screenWidth = 0;

		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		DWORD windowedStyle = {};

		if (fullscreen)
		{
		
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
			dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			width = screenWidth;
			height = screenHeight;

			posX = posY = 0;

			windowedStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}
		else
		{
		
			posX = (screenWidth - width) / 2;
			posY = (screenHeight - height) / 2;

			windowedStyle = WS_OVERLAPPEDWINDOW;

		}

		// Create the window with the screen settings and get the handle to it.
		hwnd = CreateWindowEx(
			0,                              
			windowsName,
			windowsName,
			windowedStyle,
			posX, posY, width, height,
			NULL,       
			NULL,       
			hinstance,  
			NULL        
		);

		ShowWindow(hwnd, SW_SHOW);
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);

		ShowCursor(true);


		ZeroMemory(&msg, sizeof(MSG));
	}

	void WindowsWindow::Shutdown()
	{
		Debug::CoreLog("Shutdown");
		if (fullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		DestroyWindow(hwnd);
		hwnd = NULL;


		UnregisterClass(windowsName, hinstance);
		hinstance = NULL;

		windowsWindow = NULL;
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		switch (umessage)
		{
				// Check if the window is being destroyed.
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}

			case WM_QUIT:
			{
				Debug::CoreLog("WM_QUIT");
				PostQuitMessage(0);
				return 0;
			}

			// All other messages pass to the message handler in the system class.
			default:
			{
				return windowsWindow->MessageHandler(hwnd, umessage, wparam, lparam);
			}
		}
		

	}
}

