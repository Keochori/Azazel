#pragma once
#include <chrono>

#define TIMER Timer::GetInstance()

class Timer
{
public:
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	void Initialize();
	static Timer& GetInstance();

	void Update();

	double GetTotalTime() const;
	double GetDeltaTime() const;

private:
	Timer() = default;
	~Timer() = default;

	bool initialized = false;
	std::chrono::high_resolution_clock::time_point myStartTime;
	std::chrono::high_resolution_clock::time_point myLastFrameTime;
	double myTotalTime;
	double myDeltaTime;
};

