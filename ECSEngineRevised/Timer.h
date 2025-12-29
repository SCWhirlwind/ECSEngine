#pragma once
#include <SDL.h>

class Timer
{
public:

	static Timer& Instance()
	{
		static Timer instance;
		return instance;
	}

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	void Reset();
	float DeltaTime() const;

	void TimeScale(float t);
	float TimeScale() const;

	void Update();

private:

	Timer();
	~Timer();

	unsigned int mStartTicks;
	unsigned int mElapsedTicks;
	float mDeltaTime;
	float mTimeScale;
};

