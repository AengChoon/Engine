#pragma once
#include "Camera.h"
#include "EngineTimer.h"
#include "ImguiManager.h"
#include "Model.h"
#include "PointLight.h"
#include "Window.h"

class App
{
public:
	App();
	int Run();

private:
	void DoFrame();

private:
	static inline ImGuiManager ImGui;

	Window MyWindow;
	EngineTimer MyTimer;
	Camera MyCamera;
	PointLight MyPointLight;
	float SpeedFactor {1.0f};
	std::unique_ptr<Model> Nano;
};
