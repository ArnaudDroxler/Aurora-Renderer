#pragma once


#include "Aurora/Core/Input.h"

namespace Aurora
{
	class WindowsInput : public Input
	{
	protected:


		virtual bool GetKeyDownImpl(int keycode) override;
		virtual bool GetMouseButtonImpl(int keycode) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMousePosXImpl() override;
		virtual float GetMousePosYImpl() override;
	};
}


