#include "aurorapch.h"
#include "Time.h"

#include <chrono>
#include <thread>
#include <math.h>

using namespace std::chrono;
using namespace std;

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
	/* fonction form here https://blat-blatnik.github.io/computerBear/making-accurate-sleep-function/ */
	void Time::PreciseSleep(double seconds) {


		static double estimate = 5e-3;
		static double mean = 5e-3;
		static double m2 = 0;
		static int64_t count = 1;

		while (seconds > estimate) {
			auto start = high_resolution_clock::now();
			this_thread::sleep_for(milliseconds(1));
			auto end = high_resolution_clock::now();

			double observed = (end - start).count() / 1e9;
			seconds -= observed;

			++count;
			double delta = observed - mean;
			mean += delta / count;
			m2 += delta * (observed - mean);
			double stddev = sqrt(m2 / (count - 1));
			estimate = mean + stddev;
		}

		auto start = high_resolution_clock::now();
		while ((high_resolution_clock::now() - start).count() / 1e9 < seconds);
	}

	double Time::GetHighPrecisionTime()
	{
		auto now = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(now - startTime);
		return duration.count() / 1000000.0;
	}

}
