#pragma once

#include "Aurora/Core/Window.h"
#include "Aurora/Events/ApplicationEvent.h"
#include "Aurora/Events/MouseEvent.h"
#include "Aurora/Events/KeyEvent.h"

#include "Aurora/Core/LayerStack.h"
#include "Aurora/ImGui/ImGuiLayer.h"
#include "Aurora/Core/Camera.h"


namespace Aurora
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& event);

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

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);

		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);

		std::unique_ptr<Window> window;
		LayerStack layerStack;
		ImGuiLayer* imGuiLayer;

		Camera camera;

		static Application* Instance;

		bool running;
		bool minimized;

		bool useTargetFrameRate = true;
		float targetFrameRate = 60.0f;
		float targetFrameTime = 1.0f / targetFrameRate;

		float lastMousePositionX = 0, lastMousePositionY = 0;
	};

	Application* CreateApplication();
}

