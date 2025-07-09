#pragma once

#include "Aurora/Core/Window.h"
#include "Aurora/Events/ApplicationEvent.h"

#include "Aurora/Core/LayerStack.h"
#include "Aurora/ImGui/ImGuiLayer.h"


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

		void SetUseTargetFrameRate(bool useFrameRateTarget) { useTargetFrameRate = useFrameRateTarget; }
		inline bool GetUseTargetFrameRate() const { return useTargetFrameRate; }

		void SetTargetFrameRate(float frameRate) { targetFrameRate = frameRate;  targetFrameTime = 1.0f / targetFrameRate; }
		inline float GetTargetFrameRate() const { return targetFrameRate; }

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

		bool useTargetFrameRate = true;
		float targetFrameRate = 60.0f;
		float targetFrameTime = 1.0f / targetFrameRate;
	};

	Application* CreateApplication();
}

