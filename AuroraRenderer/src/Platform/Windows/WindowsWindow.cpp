#include "aurorapch.h"
#include "AuroraRenderer.h"

#include "WindowsWindow.h"

#include "Aurora/Events/ApplicationEvent.h"
#include "Aurora/Events/MouseEvent.h"
#include "Aurora/Events/KeyEvent.h"
#include "Platform/Renderer/DirectX/DirectXContext.h"

#include "WinDef.h"
#include "windowsx.h"
#include <stdio.h>

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

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		context->BeginScene(1.0f, 0.0f, 0.f, 1.0f);

		context->SwapBuffer();
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

			case WM_KEYDOWN:
			{
				UINT key = (UINT)wParam;

				WORD repeatCount = LOWORD(lParam);

				BOOL isRepeat = (lParam & 0x40000000) != 0;

				KeyPressedEvent event = { (int) key, isRepeat ? 1 : 0 };
				eventCallback(event);
			}
			break;

			case WM_KEYUP:
			{
				UINT key = (UINT)wParam;

				KeyReleasedEvent event = { (int)key };
				eventCallback(event);
			}
			break;

			case WM_CHAR:
			{
				WCHAR ch = (WCHAR)wParam;

				KeyTypedEvent event = { (int)ch };
				eventCallback(event);
			}
			break;

			case WM_MOUSEMOVE:
			{
				float mousePosX = GET_X_LPARAM(lParam);
				float mousePosY = GET_Y_LPARAM(lParam);

				MouseMovedEvent event = { mousePosX, mousePosY };
				eventCallback(event);
			}
			break;

			case WM_LBUTTONDOWN:
			{
				float mousePosX = GET_X_LPARAM(lParam);
				float mousePosY = GET_Y_LPARAM(lParam);

				MouseButtonPressedEvent event = { (int)KeyCode::MouseLeft, mousePosX, mousePosY };
				eventCallback(event);
			}
			break;

			case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent event = { (int)KeyCode::MouseLeft };
			}
			break;

			case WM_RBUTTONDOWN:
			{
				
				float mousePosX = GET_X_LPARAM(lParam);
				float mousePosY = GET_Y_LPARAM(lParam);

				MouseButtonPressedEvent event = { (int)KeyCode::MouseRight, mousePosX, mousePosY };
				eventCallback(event);

			}
			break;

			case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent event = { (int)KeyCode::MouseRight };
			}
			break;

			case WM_MBUTTONDOWN:
			{
				float mousePosX = GET_X_LPARAM(lParam);
				float mousePosY = GET_Y_LPARAM(lParam);

				MouseButtonPressedEvent event = { (int)KeyCode::MouseMiddle, mousePosX, mousePosY };
				eventCallback(event);
			}
			break;

			case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent event = { (int)KeyCode::MouseMiddle };
			}
			break;

			case WM_MOUSEWHEEL:
			{
				float deltaY = GET_WHEEL_DELTA_WPARAM(wParam);
				float deltaX = GET_WHEEL_DELTA_WPARAM(wParam);

				MouseScrolledEvent event = { (float)deltaX, (float)deltaY };
			}
			break;


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

			}
			break;

			default:
			{
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			return 0;
		}
	}


	void WindowsWindow::Init(const WindowProperty& props)
	{

		title = props.Title;
		height = props.Height;
		width = props.Width;
		fullscreen = props.Fullscreen;

		WNDCLASSEX wc = {};

		hinstance = GetModuleHandle(nullptr);

		std::wstring titleWStr(title.begin(), title.end());
		windowsName = titleWStr.c_str();

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = Aurora::WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = nullptr;
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
			nullptr,       
			nullptr,       
			hinstance,  
			this       
		);

		ShowWindow(hwnd, SW_SHOW);
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);

		ShowCursor(true);


		ZeroMemory(&msg, sizeof(MSG));

		context = new DirectXContext();
		context->InitDirectX(width, height, vSync, hwnd, fullscreen, 1000.0f, 0.1f);

	}

	void WindowsWindow::Shutdown()
	{

		context->Shutdown();
		context = nullptr;

		if (fullscreen)
		{
			ChangeDisplaySettings(nullptr, 0);
		}

		DestroyWindow(hwnd);
		hwnd = nullptr;

		UnregisterClass(windowsName, hinstance);
		hinstance = nullptr;

	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowsWindow* window = nullptr;
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

