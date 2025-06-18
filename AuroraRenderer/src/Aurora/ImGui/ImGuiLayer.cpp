#include "aurorapch.h"
#include "ImGuiLayer.h"

#include "Aurora/Core/Application.h"
#include "Aurora/Core/Window.h"
#include "Platform/Windows/WindowsWindow.h"


#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace Aurora
{


	ImGuiLayer::ImGuiLayer() : Layer("Imgui Layer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


		Application& app = Application::Get();

		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui_ImplWin32_Init(app.GetWindow().GetNativeWindow());

		WindowsWindow* windowsWindow = dynamic_cast<WindowsWindow*>(app.GetWindowPtr());
		ImGui_ImplDX11_Init(windowsWindow->GetContext()->GetDevice(), windowsWindow->GetContext()->GetDeviceContext());

		
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGUIRender()
	{

	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&ImGuiLayer::OnWindowResize, this, std::placeholders::_1));
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{


		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	

	}

	bool ImGuiLayer::OnWindowResize(WindowResizeEvent& event)
	{

		if (ImGui::GetCurrentContext())
		{
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)event.GetWidth(), (float)event.GetHeight());

			Debug::CoreLog("ImGuiLaye::OnWindowResize {0} {1} ", io.DisplaySize.x, io.DisplaySize.y);
		}
		return false;
	}


}
