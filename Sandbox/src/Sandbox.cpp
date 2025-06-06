#include "AuroraRenderer.h"
#include "Aurora/Core/EntryPoint.h"
#include "Aurora/Core/Debug.h"

using namespace Aurora;

class SandboxApp : public Application
{
public:
	SandboxApp()
	{
		Debug::Log("Hello from SandboxApp");
	}

	~SandboxApp()
	{

	}

};


Aurora::Application* Aurora::CreateApplication()
{
	return new SandboxApp();
}

