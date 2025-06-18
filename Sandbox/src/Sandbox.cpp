#include "AuroraRenderer.h"
#include "Aurora/Core/EntryPoint.h"

#include "OverlayLayer.h" 
#include "SettingsLayer.h"

#include "Platform/Renderer/DirectX/DirectXContext.h"

#include "imgui.h"


using namespace Aurora;


class SandboxApp : public Application
{
public:
	SandboxApp()
	{
		Debug::Log("Hello from SandboxApp");
		PushLayer(new OverlayLayer());
        PushLayer(new SettingsLayer());
	}

	~SandboxApp()
	{

	}

};


Aurora::Application* Aurora::CreateApplication()
{
	return new SandboxApp();
}

