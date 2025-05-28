#pragma once


extern Aurora::Application* Aurora::CreateApplication();

int main(int argc, char** argv)
{

	auto app = Aurora::CreateApplication();
	app->Run();
	delete app;

}