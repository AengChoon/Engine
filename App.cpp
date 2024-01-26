#include "App.h"
#include "GDIPlusManager.h"

GDIPlusManager GDIPlus;

App::App()
	: MyWindow(800, 600, WindowClass::GetName())
	, MyPointLight(MyWindow.GetGraphics())
{
	MyWindow.GetGraphics().SetCamera(MyCamera);
	MyWindow.GetGraphics().SetProjectionMatrix(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int App::Run()
{
	while (true)
	{
		if (const auto ExitCode = Window::ProcessMessages())
		{
			return ExitCode.value();
		}

		DoFrame();
	}
}

void App::DoFrame()
{
	auto DeltaTime = MyTimer.Mark() * SpeedFactor;

	MyWindow.GetGraphics().BeginFrame();

	if (!Nano) { Nano = std::make_unique<Model>(MyWindow.GetGraphics(),"Models\\nanosuit.obj"); }

	MyPointLight.Bind(MyWindow.GetGraphics(), MyCamera.GetMatrix());

	Nano->Draw(MyWindow.GetGraphics());

	MyPointLight.Draw(MyWindow.GetGraphics());
	MyCamera.ShowControlWindow();
	MyPointLight.ShowControlWindow();
	Nano->ShowWindow();
	MyWindow.GetGraphics().EndFrame();
}