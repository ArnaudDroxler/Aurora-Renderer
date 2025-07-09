#pragma once
#include <chrono>

using namespace std::chrono;

namespace Aurora
{

    class Time
    {

    public:

        static void Init();

        static void Update();

        static float DeltaTime() { return deltaTime; }
        static float TotalTime() { return totalTime; }
        static int FrameCount() { return frameCount; }
        static float FPS() { return fps; }

        static void SetFixedDeltaTime(float deltaTime) { fixedDeltaTime = deltaTime; }


        static void Sleep(float seconds);
        static void PreciseSleep(double seconds);
        static double GetHighPrecisionTime();

    private:

        static high_resolution_clock::time_point startTime;
        static high_resolution_clock::time_point lastFrameTime;
        static high_resolution_clock::time_point currentFrameTime;
        static float deltaTime;
        static float totalTime;
        static float fixedDeltaTime;
        static int frameCount;
        static float fps;
        static float fpsTimer;
        static int fpsFrameCount;

    };

}