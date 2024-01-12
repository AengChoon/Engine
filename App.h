#pragma once
#include "Window.h"

class App
{
public:
	App() : MyWindow(800, 600, WindowClass::GetName())
	{}

	int Run();

private:
	void DoFrame();

private:
	Window MyWindow;
};
