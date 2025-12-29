#include "Timer.h"

Timer::Timer()
{
    Reset();
    mElapsedTicks = 0;
    mDeltaTime = 0.0f;
    mTimeScale = 1.0f;
}

Timer::~Timer() {}

void Timer::Reset()
{
    mStartTicks = SDL_GetTicks();

}

float Timer::DeltaTime() const
{
    return mDeltaTime;
}

void Timer::TimeScale(float t)
{
    mTimeScale = t;
}

float Timer::TimeScale() const
{
    return mTimeScale;
}

void Timer::Update()
{
    mElapsedTicks = SDL_GetTicks() - mStartTicks;
    mDeltaTime = (mElapsedTicks * 0.001f) * mTimeScale;
}