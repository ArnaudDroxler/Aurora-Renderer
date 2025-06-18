#pragma once
#include "Aurora/Core/Layer.h"
#include "Aurora/Core/LayerStack.h"
#include "Aurora/Events/Event.h"
#include "Aurora/Events/ApplicationEvent.h"

#include "imgui.h"
#include "imgui_internal.h"



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

		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		static inline ImGuiWindow* window = nullptr;

	private :

		bool OnWindowResize(WindowResizeEvent& event);

	};

}