#include "aurorapch.h"
#include "ImGuiLayer.h"

#include "Aurora/Core/Application.h"
#include "Aurora/Core/Window.h"
#include "Platform/Windows/WindowsWindow.h"

#include "imgui.h"
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
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		//ImGui::StyleColorsDark();


		ImGui_ImplWin32_Init(Application::Get().GetWindow().GetNativeWindow());

		WindowsWindow* windowsWindow = dynamic_cast<WindowsWindow*>(Application::Get().GetWindowPtr());
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


}
