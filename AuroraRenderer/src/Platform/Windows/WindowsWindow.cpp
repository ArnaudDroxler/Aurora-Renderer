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
#include <algorithm>

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

	void WindowsWindow::SetWindowVisibility(bool visibility)
	{
		if (visibility)
		{
			ShowWindow(hwnd, SW_SHOW);
			SetForegroundWindow(hwnd);
		}
		else
		{
			ShowWindow(hwnd, SW_HIDE);
			
		}
	}

	void WindowsWindow::SetCursorVisibility(bool visibility)
	{
		if (visibility)
		{
			ShowCursor(true);

			SetFocus(hwnd);
		}
		else
		{
			ShowCursor(false);
		}
	}

	void WindowsWindow::OnUpdate()
	{

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		context->SwapBuffer();
	}

	void WindowsWindow::OnNewFrame()
	{

		context->ClearColor(0.1f, 0.1f, 0.1f, 0.1f);

	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		vSync = enabled;
		context->SetVSync(vSync);
	}

	void WindowsWindow::SetWindowMode(WindowMode mode)
	{
		windowMode = mode;

		switch (mode)
		{
			case WindowMode::Windowed:
			{
				ChangeDisplaySettings(nullptr, 0);

				SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

				SetWindowPos(hwnd, nullptr, posX, posY, width, height, SWP_FRAMECHANGED | SWP_NOZORDER);
			}break;
			case WindowMode::BorderlessFullscreen:
			{
				ChangeDisplaySettings(nullptr, 0); 

				LONG style = GetWindowLong(hwnd, GWL_STYLE);
				style &= ~(WS_OVERLAPPEDWINDOW);
				SetWindowLong(hwnd, GWL_STYLE, style);

				int screenWidth = GetSystemMetrics(SM_CXSCREEN);
				int screenHeight = GetSystemMetrics(SM_CYSCREEN);

				width = screenWidth;
				height = screenHeight;

				posX = 0;
				posY = 0;

				SetWindowPos(hwnd, HWND_TOP, posX, posY, width, height, SWP_FRAMECHANGED | SWP_NOZORDER);

				context->SetFullscreen(true);
			} break;
			case WindowMode::ExclusiveFullscreen:
			{
				DisplayMode mode = GetCurrentDisplayMode();
				DEVMODE dm = {};
				dm.dmSize = sizeof(DEVMODE);
				dm.dmPelsWidth = mode.width;
				dm.dmPelsHeight = mode.height;
				dm.dmBitsPerPel = 32;
				dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

				ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
				SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);

				SetWindowPos(hwnd, HWND_TOP, 0, 0, mode.width, mode.height, SWP_FRAMECHANGED | SWP_NOZORDER);

				context->SetFullscreen(true);
			} break;
		}

		SetWindowVisibility(true);
	}


	void WindowsWindow::SetResolutionAndRefreshRate(DisplayMode displayMode)
	{
		Debug::CoreLog("WindowsWindow::SetResolutionAndRefreshRate Setting resolution to {0}x{1} @ {2}Hz", displayMode.width, displayMode.height, displayMode.GetRefreshRate());

		Debug::CoreLog("WindowsWindow::SetResolutionAndRefreshRate Old position: ({0}, {1})", posX, posY);

		context->SetResolutionAndRefreshRate(displayMode);

		if (windowMode == WindowMode::Windowed)
		{
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);

			RECT rect = { 0, 0, (long)displayMode.width, (long)displayMode.height };

			AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

			int adjustedWidth = rect.right - rect.left;
			int adjustedHeight = rect.bottom - rect.top;

			posX = (screenWidth - adjustedWidth) / 2;
			posY = (screenHeight - adjustedHeight) / 2;

			Debug::CoreLog("WindowsWindow::SetResolutionAndRefreshRate Adjusted position: ({0}, {1})", posX, posY);

			SetWindowPos(hwnd,
				nullptr,
				posX, posY,
				adjustedWidth,
				adjustedHeight,
				SWP_NOZORDER);
		}
	


	}

	void WindowsWindow::SetEventCallback(const EventCallback& callback)
	{
		eventCallback = callback;
	}

	DisplayMode WindowsWindow::GetCurrentDisplayMode() const
	{
		return context->GetCurrentDisplayMode();
	}

	void WindowsWindow::SetNativeWindow(HWND hwnd)
	{
		this->hwnd = hwnd;
	}

	std::vector<std::pair<unsigned int, unsigned int>> WindowsWindow::GetResolutions() const
	{
		std::vector<std::pair<UINT, UINT>> resolutions;

		for (const auto& mode : displayModes)
		{
			std::pair<UINT, UINT> res = { mode.width, mode.height };

			bool exists = false;
			for (const auto& resolution : resolutions)
			{
				if (resolution.first == res.first && resolution.second == res.second)
				{
					exists = true;
					break;
				}
			}

			if (!exists)
			{
				resolutions.push_back(res);
			}
		}

		return resolutions;
	}

	std::vector< std::pair<unsigned int, unsigned int>> WindowsWindow::GetRefreshRatesForResolution(unsigned int width, unsigned int height) const
	{
		std::vector<std::pair<unsigned int, unsigned int>> refreshRates;

		for (const auto& mode : displayModes)
		{
			if (mode.width == width && mode.height == height)
			{
				std::pair<unsigned int, unsigned int> refreshRate = {mode.refreshRateNumerator, mode.refreshRateDenominator };

				bool exists = false;
				for (auto rRate : refreshRates)
				{
					if (rRate.first == refreshRate.first && rRate.second == refreshRate.second)
					{
						exists = true;
						break;
					}
				}

				if (!exists)
				{
					refreshRates.push_back(refreshRate);
				}
			}
		}

		std::sort(refreshRates.begin(), refreshRates.end(),
			[](const std::pair<unsigned int, unsigned int>& a, const std::pair<unsigned int, unsigned int>& b) {
				float freqA = (float)a.first / a.second;
				float freqB = (float)b.first / b.second;
				return freqA < freqB;
			});

		return refreshRates;
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
				eventCallback(event);
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
				eventCallback(event);
			}
			break;

			case WM_MOUSEWHEEL:
			{
				float deltaY = GET_WHEEL_DELTA_WPARAM(wParam);
				float deltaX = GET_WHEEL_DELTA_WPARAM(wParam);

				MouseScrolledEvent event = { (float)deltaX, (float)deltaY };
				eventCallback(event);
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
				
				this->width = width;
				this->height = height;

				this->context->OnResize(width, height);

				WindowResizeEvent event = { width, height };
				eventCallback(event);
			
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
		windowMode = props.windowMode;

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

		if (windowMode == WindowMode::ExclusiveFullscreen)
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

		}
		else if (windowMode == WindowMode::Windowed)
		{
		
			RECT rect = { 0, 0, (LONG)width, (LONG)height };
			windowedStyle = WS_OVERLAPPEDWINDOW;

			AdjustWindowRectEx(&rect, windowedStyle, FALSE, 0);

			int adjustedWidth = rect.right - rect.left;
			int adjustedHeight = rect.bottom - rect.top;

			posX = (screenWidth - adjustedWidth) / 2;
			posY = (screenHeight - adjustedHeight) / 2;

			hwnd = CreateWindowEx(
				0,
				windowsName,
				windowsName,
				windowedStyle,
				posX, posY, adjustedWidth, adjustedHeight,  
				nullptr,
				nullptr,
				hinstance,
				this
			);

		}


		msg = {};

		vSync = false;

		context = new DirectXContext();
		context->InitDirectX(width, height, vSync, hwnd, windowMode == WindowMode::ExclusiveFullscreen || windowMode == WindowMode::BorderlessFullscreen, 1000.0f, 0.1f);

		displayModes = context->GetDisplayModes();

	}

	void WindowsWindow::Shutdown()
	{

		context->Shutdown();
		context = nullptr;

		if (windowMode == WindowMode::ExclusiveFullscreen || windowMode == WindowMode::BorderlessFullscreen)
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


		if (ImGui::GetCurrentContext() != nullptr)
		{
			if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			{
				return true;
			}
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
