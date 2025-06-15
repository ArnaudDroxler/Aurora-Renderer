#include "aurorapch.h"
#include "Time.h"

using namespace std::chrono;

namespace Aurora
{

	std::chrono::high_resolution_clock::time_point Time::startTime;
	std::chrono::high_resolution_clock::time_point Time::lastFrameTime;
	std::chrono::high_resolution_clock::time_point Time::currentFrameTime;
	float Time::deltaTime = 0.0f;
	float Time::fixedDeltaTime = 1.0f / 60.0f;
	float Time::totalTime = 0.0f;
	int Time::frameCount = 0;
	float Time::fps = 0.0f;
	float Time::fpsTimer = 0.0f;
	int Time::fpsFrameCount = 0;


	void Time::Init()
	{
		startTime = high_resolution_clock::now();
		lastFrameTime = startTime;
		currentFrameTime = startTime;
		deltaTime = 0.0f;
		totalTime = 0.0f;
		frameCount = 0;
		fps = 0.0f;
		fpsTimer = 0.0f;
		fpsFrameCount = 0;
		fixedDeltaTime = 1.0f/60.0f;
	}

	void Time::Update()
	{
		lastFrameTime = currentFrameTime;
		currentFrameTime = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(currentFrameTime - lastFrameTime);
		deltaTime = duration.count() / 1000000.0f;

		auto totalDuration = duration_cast<microseconds>(currentFrameTime - startTime);
		totalTime = totalDuration.count() / 1000000.0f;

		frameCount++;

		fpsTimer += deltaTime;
		fpsFrameCount++;

		if (fpsTimer >= 0.1f)
		{
			fps = fpsFrameCount / fpsTimer;
			fpsTimer = 0.0f;
			fpsFrameCount = 0;
		}
	}

	void Time::Sleep(float seconds)
	{
		if (seconds > 0.0f)
		{
			::Sleep(static_cast<DWORD>(seconds * 1000.0f));
		}
	}

	double Time::GetHighPrecisionTime()
	{
		auto now = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(now - startTime);
		return duration.count() / 1000000.0;
	}

}
