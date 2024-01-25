#include "App.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"

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

	const auto ModelTransform = DirectX::XMMatrixRotationRollPitchYaw(Position.roll,Position.pitch,Position.yaw) *
										DirectX::XMMatrixTranslation(Position.x,Position.y,Position.z);
	Nano->Draw(MyWindow.GetGraphics(), ModelTransform);

	MyPointLight.Draw(MyWindow.GetGraphics());
	MyCamera.ShowControlWindow();
	MyPointLight.ShowControlWindow();
	ShowModelWindow();
	MyWindow.GetGraphics().EndFrame();
}

void App::ShowModelWindow()
{
	if( ImGui::Begin( "Model" ) )
	{
		using namespace std::string_literals;

		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll",&Position.roll,-180.0f,180.0f );
		ImGui::SliderAngle( "Pitch",&Position.pitch,-180.0f,180.0f );
		ImGui::SliderAngle( "Yaw",&Position.yaw,-180.0f,180.0f );

		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X",&Position.x,-20.0f,20.0f );
		ImGui::SliderFloat( "Y",&Position.y,-20.0f,20.0f );
		ImGui::SliderFloat( "Z",&Position.z,-20.0f,20.0f );
	}
	ImGui::End();
}
