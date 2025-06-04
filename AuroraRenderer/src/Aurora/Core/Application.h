#pragma once

#include "Aurora/Core/Window.h"
#include "Aurora/Events/ApplicationEvent.h"

namespace Aurora
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void Run();

		inline Window& GetWindow() { return *window; }

		inline static Application& Get() { return *Instance; }

	private: 

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> window;

		static Application* Instance;

		bool running;
	};

	Application* CreateApplication();
}

