#include "aurorapch.h"
#include "WindowsInput.h"


namespace Aurora
{

	Input* Input::Instance = new WindowsInput();

	bool WindowsInput::GetKeyDownImpl(int keycode)
	{
		return (GetAsyncKeyState(keycode) & 0x8000) != 0;
	}



	bool WindowsInput::GetMouseButtonImpl(int keycode)
	{
		return (GetAsyncKeyState(keycode) & 0x8000) != 0;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		POINT mousePos;
		if (GetCursorPos(&mousePos))
		{

			HWND hwnd = (HWND)Application::Get().GetWindow().GetNativeWindow();
			ScreenToClient(hwnd, &mousePos);
			return std::make_pair(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		}

		return std::make_pair(0.0f, 0.0f);
	}

	float WindowsInput::GetMousePosXImpl()
	{
		POINT mousePos;
		if (GetCursorPos(&mousePos))
		{
			HWND hwnd = (HWND)Application::Get().GetWindow().GetNativeWindow();
			ScreenToClient(hwnd, &mousePos);
			return static_cast<float>(mousePos.x);
		}

		return 0.0f;
	}

	float WindowsInput::GetMousePosYImpl()
	{
		POINT mousePos;
		if (GetCursorPos(&mousePos))
		{
			HWND hwnd = (HWND)Application::Get().GetWindow().GetNativeWindow();
			ScreenToClient(hwnd, &mousePos);

			return static_cast<float>(mousePos.y);
		}

		return 0.0f;
	}

}
