#include "pch.h"
#include "Timer.h"

void Timer::Initialize()
{
	myStartTime = std::chrono::high_resolution_clock::now();
	myLastFrameTime = myStartTime;
	myTotalTime = 0;
	myDeltaTime = 0;
	initialized = true;
}

Timer& Timer::GetInstance()
{
	static Timer ourInstance;
	return ourInstance;
}

void Timer::Update()
{
	if (!initialized)
	{
		LOG_WARNING("Timer called Update() before being properly initialized.");
		Initialize();
	}

	myTotalTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - myStartTime).count();
	myDeltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - myLastFrameTime).count();
	myLastFrameTime = std::chrono::high_resolution_clock::now();
}

double Timer::GetTotalTime() const
{
    return myTotalTime;
}

double Timer::GetDeltaTime() const
{
	return myDeltaTime;
}