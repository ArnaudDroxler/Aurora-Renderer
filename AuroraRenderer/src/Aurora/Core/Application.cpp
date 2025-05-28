#include "aurorapch.h"
#include "Application.h"
#include "Debug.h"

namespace Aurora
{
	Application::Application()
	{
		Debug::Init();

		Debug::CoreInfo("Aurora Renderer Init");
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{

		while (running)
		{

			Debug::CoreInfo("Run");

		}

	}


}

