#include "EngineTimer.h"

EngineTimer::EngineTimer() noexcept
{
	Last = std::chrono::steady_clock::now();
}

float EngineTimer::Mark() noexcept
{
	const auto old = Last;
	Last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> FrameTime = Last - old;
	return FrameTime.count();
}

float EngineTimer::Peek() const noexcept
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - Last).count();
}