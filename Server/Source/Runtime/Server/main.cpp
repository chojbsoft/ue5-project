#include "pch.h"
#include "engine.h"

int main()
{
	// 엔진 초기화
	int32 errorLevel = engine.Init();
	if (errorLevel != 0)
	{
		return errorLevel;
	}

	// 엔진 Tick
	auto lastTime = std::chrono::high_resolution_clock::now();
	while (!engine._isExitRequested)
	{
		auto currTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currTime - lastTime;
		lastTime = currTime;

		engine.Tick(deltaTime.count());
	}

	// 엔진 Exit
	engine.Exit();

	return errorLevel;
}
