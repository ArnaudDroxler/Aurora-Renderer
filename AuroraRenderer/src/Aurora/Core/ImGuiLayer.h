#pragma once
#include "Aurora/Core/Layer.h"
#include "Aurora/Core/LayerStack.h"
#include "Aurora/Events/Event.h"

namespace Aurora
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGUIRender() override;

		void Begin();
		void End();
	};

}