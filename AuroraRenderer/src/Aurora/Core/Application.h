#pragma once

#include "Aurora/Core/Window.h"
#include "Aurora/Events/ApplicationEvent.h"

#include "Aurora/Core/LayerStack.h"
#include "Aurora/Core/ImGuiLayer.h"


namespace Aurora
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *window; }
		inline Window* GetWindowPtr() { return window.get(); }

		inline static Application& Get() { return *Instance; }

	private: 

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> window;
		LayerStack layerStack;
		ImGuiLayer* imGuiLayer;


		static Application* Instance;


		bool running;
		bool minimized;
	};

	Application* CreateApplication();
}

