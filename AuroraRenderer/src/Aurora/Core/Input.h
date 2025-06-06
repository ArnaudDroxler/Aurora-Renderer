#pragma once

#include "AuroraRenderer.h"

namespace Aurora {

	class  Input
	{
	public:

		inline static bool GetKeyDown(KeyCode keycode) { return Instance->GetKeyDownImpl((int)keycode); };
		inline static bool GetKeyDown(int keycode) { return Instance->GetKeyDownImpl(keycode); };

		inline static bool GetMouseButton(int button) { return Instance->GetMouseButtonImpl(button); };

		inline static std::pair<float, float> GetMousePosition() { return Instance->GetMousePositionImpl(); }
		inline static float GetMousePosX() { return Instance->GetMousePosXImpl(); };
		inline static float GetMousePosY() { return Instance->GetMousePosYImpl(); };

	protected:
		virtual bool GetKeyDownImpl(int keycode) = 0;

		virtual bool GetMouseButtonImpl(int button) = 0;

		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMousePosXImpl() = 0;
		virtual float GetMousePosYImpl() = 0;

	private:
		static Input* Instance;
	};


}
