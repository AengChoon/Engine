#pragma once
#include "Drawable.h"
#include "EngineTimer.h"
#include "ImguiManager.h"
#include "Window.h"
#include "Camera.h"
#include "PointLight.h"

class App
{
public:
	App();
	int Run();

private:
	void DoFrame();

private:
	static inline ImGuiManager ImGui;
	static constexpr size_t DrawablesNum {180};

	Window MyWindow;
	EngineTimer MyTimer;
	Camera MyCamera;
	PointLight MyPointLight;
	std::vector<std::unique_ptr<Drawable>> Drawables;
	float SpeedFactor {1.0f};
};
