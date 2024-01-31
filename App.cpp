#include "App.h"
#include "GDIPlusManager.h"

GDIPlusManager GDIPlus;

App::App()
	: MyWindow(1280, 720, WindowClass::GetName())
	, MyPointLight(MyWindow.GetGraphics())
{
	MyWindow.GetGraphics().SetCamera(MyCamera);
	MyWindow.GetGraphics().SetProjectionMatrix(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
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

	if (!Nano) { Nano = std::make_unique<Model>(MyWindow.GetGraphics(),"Models\\nanosuit.gltf"); }

	MyPointLight.Bind(MyWindow.GetGraphics(), MyCamera.GetMatrix());
	Nano->ShowWindow();
	Nano->Draw(MyWindow.GetGraphics());

	while (const auto Event = MyWindow.MyKeyboard.ReadKey())
	{
		if (Event->IsPress() && Event->GetCode() == VK_MENU)
		{
			if (MyWindow.IsCursorEnabled())
			{
				MyWindow.DisableCursor();
				MyWindow.MyMouse.EnableRawInput();
			}
			else
			{
				MyWindow.EnableCursor();
				MyWindow.MyMouse.DisableRawInput();
			}
		}
	}

	if (!MyWindow.IsCursorEnabled())
	{
		if (MyWindow.MyKeyboard.IsKeyPressed('W'))
		{
			MyCamera.Translate({0.0f, 0.0f, DeltaTime});
		}
		if (MyWindow.MyKeyboard.IsKeyPressed('A'))
		{
			MyCamera.Translate({-DeltaTime, 0.0f, 0.0f});
		}
		if (MyWindow.MyKeyboard.IsKeyPressed('S'))
		{
			MyCamera.Translate({0.0f, 0.0f, -DeltaTime});
		}
		if (MyWindow.MyKeyboard.IsKeyPressed('D'))
		{
			MyCamera.Translate({DeltaTime, 0.0f, 0.0f});
		}
		if (MyWindow.MyKeyboard.IsKeyPressed('R'))
		{
			MyCamera.Translate({0.0f, DeltaTime, 0.0f});
		}
		if (MyWindow.MyKeyboard.IsKeyPressed('F'))
		{
			MyCamera.Translate({0.0f, -DeltaTime, 0.0f});
		}
	}

	while (const auto RawDelta = MyWindow.MyMouse.ReadRawDelta())
	{
		if (!MyWindow.IsCursorEnabled())
		{
			MyCamera.Rotate(static_cast<float>(RawDelta->DeltaX), static_cast<float>(RawDelta->DeltaY));
		}
	}

	MyPointLight.ShowControlWindow();
	MyPointLight.Draw(MyWindow.GetGraphics());
	MyCamera.ShowControlWindow();
	MyWindow.GetGraphics().EndFrame();
}