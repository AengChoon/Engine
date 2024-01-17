#pragma once
#include "Drawable.h"
#include "EngineTimer.h"
#include "Window.h"

class App
{
public:
	App();
	int Run();

private:
	void DoFrame();

private:
	Window MyWindow;
	EngineTimer MyTimer;
	std::vector<std::unique_ptr<Drawable>> Drawables;
	static constexpr size_t DrawablesNum = 180;
};
