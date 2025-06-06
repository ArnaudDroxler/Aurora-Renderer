#include "aurorapch.h"
#include "WindowsWindow.h"
#include "Aurora/Events/ApplicationEvent.h"
#include "WinDef.h"


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
		eventCallback = callback;
	}

	void WindowsWindow::SetNativeWindow(HWND hwnd)
	{
		this->hwnd = hwnd;
	}

	LRESULT CALLBACK WindowsWindow::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		switch (uMsg)
		{

			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
			
			case WM_CLOSE:
			{
				WindowCloseEvent event;
				eventCallback(event);
				return 0;
			}

			case WM_SIZE:
			{
				unsigned int width = LOWORD(lParam);
				unsigned int height = HIWORD(lParam);
				
				if (eventCallback)
				{
					WindowResizeEvent event = { width, height };
					eventCallback(event);
				}

				return 0;
			}

			default:
			{
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			return TRUE;
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
			this       
		);

		ShowWindow(hwnd, SW_SHOW);
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);

		ShowCursor(true);


		ZeroMemory(&msg, sizeof(MSG));
	}

	void WindowsWindow::Shutdown()
	{
		if (fullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		DestroyWindow(hwnd);
		hwnd = NULL;

		UnregisterClass(windowsName, hinstance);
		hinstance = NULL;

	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowsWindow* window = NULL;
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* create = (CREATESTRUCT*)lParam;
			window = (WindowsWindow*)create->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);

			window->SetNativeWindow(hwnd);
		}
		else
		{
			window = (WindowsWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		if (window)
		{
			return window->MessageHandler(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

	}


	inline WindowsWindow* GetAppState(HWND hwnd)
	{
		LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
		WindowsWindow* window = reinterpret_cast<WindowsWindow*>(ptr);
		return window;
	}
}

