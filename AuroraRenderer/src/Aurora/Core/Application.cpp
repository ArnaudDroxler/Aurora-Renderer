#include "aurorapch.h"
#include "Application.h"
#include "AuroraRenderer.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Aurora
{

	Application* Application::Instance = nullptr;

	Application::Application()
	{

		Time::Init();

		Debug::Init();
		Debug::CoreLog("Aurora Renderer Init");

		Instance = this;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		window->SetWindowVisibility(true);
		window->SetCursorVisibility(true);

		imGuiLayer = new ImGuiLayer();
		PushOverlay(imGuiLayer);

		camera = Camera(Vector3(0.0f, 0.0f, -3.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));

		running = true;
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& event)
	{	

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&Application::OnKeyPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&Application::OnKeyReleased, this, std::placeholders::_1));

		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Application::OnMouseMoved, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&Application::OnMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Application::OnMouseButtonPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&Application::OnMouseButtonReleased, this, std::placeholders::_1));

		for (auto it = layerStack.end(); it != layerStack.begin(); )
		{
			--it;
			(*it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Application::Run()
	{

		while (running)
		{
			auto frameStart = Time::GetHighPrecisionTime();

			Time::Update();

			window->OnNewFrame();

			for (Layer* layer : layerStack)
				layer->OnUpdate();

			imGuiLayer->Begin();
			for (Layer* layer : layerStack)
				layer->OnImGUIRender();
			imGuiLayer->End();

			window->OnUpdate();

			auto frameEnd = Time::GetHighPrecisionTime();
			float frameTime = frameEnd - frameStart;
			
			if (useTargetFrameRate)
			{
				if (frameTime < targetFrameTime)
				{
					float sleepTime = targetFrameTime - frameTime;
					Time::PreciseSleep(sleepTime);
				}
			}
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		layerStack.PushOverlay(layer);
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		running = false;

		Debug::Shutdown();

		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{

		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			minimized = true;
			return false;
		}

		minimized = false;

		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& event)
	{
		KeyCode keyCode = (KeyCode)event.GetKeyCode();
		switch (keyCode)
		{
		case KeyCode::W:
			camera.MoveForward(1);
			break;
		case KeyCode::S:
			camera.MoveForward(-1);
			break;
		case KeyCode::A:
			camera.MoveRight(-1);
			break;
		case KeyCode::D:
			camera.MoveRight(1);
			break;
		case KeyCode::Q:
			camera.MoveUp(1);
			break;
		case KeyCode::E:
			camera.MoveUp(-1);
			break;
		default:
			break;
		}
		return false;
	}

	bool Application::OnKeyReleased(KeyReleasedEvent& event)
	{
		return false;
	}

	bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		return false;
	}

	bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
	{
		return false;
	}

	bool Application::OnMouseMoved(MouseMovedEvent& event)
	{

		float posX = event.GetMouseX();
		float posY = event.GetMouseY();

		float deltaX = lastMousePositionX - posX;
		float deltaY = lastMousePositionY - posY;

		camera.Rotate(deltaX, deltaY);

		return false;
	}

	bool Application::OnMouseScrolled(MouseScrolledEvent& event)
	{

		float delta = event.GetY();

		camera.Zoom(delta);

		return false;
	}


}

