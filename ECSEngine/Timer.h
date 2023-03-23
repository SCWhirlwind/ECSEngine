#pragma once
#include <SDL.h>

class Timer
{
public:

	static Timer* Instance();
	static void Release();

	void Reset();
	float DeltaTime();

	void TimeScale(float t = 1.0f);
	float TimeScale();

	void Update();

private:

	Timer();
	~Timer();

	static Timer* sInstance;

	unsigned int mStartTicks;
	unsigned int mElapsedTicks;
	float mDeltaTime;
	float mTimeScale;
};

