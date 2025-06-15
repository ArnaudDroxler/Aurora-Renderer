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


		imGuiLayer = new ImGuiLayer();
		PushOverlay(imGuiLayer);

		running = true;
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		Debug::CoreLog(e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

		for (auto it = layerStack.end(); it != layerStack.begin(); )
		{
			--it;
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{

		while (running)
		{
			Time::Update();

			window->OnNewFrame();

			for (Layer* layer : layerStack)
				layer->OnUpdate();

			imGuiLayer->Begin();
			for (Layer* layer : layerStack)
				layer->OnImGUIRender();
			imGuiLayer->End();

			window->OnUpdate();

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

		WindowsWindow* windowsWindow = dynamic_cast<WindowsWindow*>(window.get());
		if (windowsWindow && windowsWindow->GetContext())
		{
			windowsWindow->GetContext()->OnResize(event.GetWidth(), event.GetHeight());
		}

		if (imGuiLayer && ImGui::GetCurrentContext())
		{
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)event.GetWidth(), (float)event.GetHeight());

			Debug::CoreLog("ImGui DisplaySize upted {0} {1} ", io.DisplaySize.x, io.DisplaySize.y);
		}

		return false;
	}


}

