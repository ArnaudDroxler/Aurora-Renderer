#include "AuroraRenderer.h"
#include "Aurora/Core/EntryPoint.h"
#include "Aurora/Core/Debug.h"

class SandboxApp : public Aurora::Application
{
public:
	SandboxApp()
	{
		Aurora::Debug::Info("Hello from SandboxApp");
	}

	~SandboxApp()
	{

	}

};


Aurora::Application* Aurora::CreateApplication()
{
	return new SandboxApp();
}

