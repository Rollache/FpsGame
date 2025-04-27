#pragma once
#include <chrono>

class Timer
{
public:
	Timer()
	{
		reset();
	}
	~Timer() = default;

	//リセット
	void reset()
	{
		startTime = std::chrono::high_resolution_clock::now();
	}

	//経過時間を取得
	float getElapsedSeconds()
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = now - startTime;
		return elapsed.count();
	}

private:
	std::chrono::high_resolution_clock::time_point startTime;
};

