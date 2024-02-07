#pragma once
#include "Camera.h"
#include "EngineTimer.h"
#include "ImguiManager.h"
#include "Mesh.h"
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
	float SpeedFactor {1.0f};
	std::unique_ptr<PointLight> Light;
	std::unique_ptr<Model> Nano;
	std::unique_ptr<Model> Nano2;
};
