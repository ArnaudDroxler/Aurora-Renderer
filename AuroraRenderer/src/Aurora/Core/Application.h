#pragma once

namespace Aurora
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private: 


		bool running;
	};

	Application* CreateApplication();
}

