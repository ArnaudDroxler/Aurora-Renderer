#include "aurorapch.h"
#include "Application.h"
#include "AuroraRenderer.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Aurora
{

	Application* Application::Instance = nullptr;

	Application::Application()
	{
		Debug::Init();

		Debug::CoreLog("Aurora Renderer Init");

		Instance = this;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		running = true;
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

	}

	void Application::Run()
	{

		while (running)
		{
			window->OnUpdate();

		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		running = false;

		Debug::Shutdown();

		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		Debug::CoreLog(event.ToString());
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			minimized = true;
			return false;
		}

		minimized = false;

		return false;
	}


}

