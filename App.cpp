#include "App.h"
#include <random>
#include "AssetTest.h"
#include "Ball.h"
#include "Box.h"
#include "EngineMath.h"
#include "GDIPlusManager.h"
#include "DynamicVertex.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

GDIPlusManager GDIPlus;

App::App()
	: MyWindow(800, 600, WindowClass::GetName()), MyPointLight(MyWindow.GetGraphics())
{
	MyWindow.GetGraphics().SetCamera(MyCamera);
	class Factory
	{
	public:
		Factory(Graphics& InGraphics)
			: MyGraphics(InGraphics)
		{}

		std::unique_ptr<Drawable> operator()()
		{
			const DirectX::XMFLOAT3 MaterialColor = {Color(RandomGenerator), Color(RandomGenerator), Color(RandomGenerator)};
			return std::make_unique<AssetTest>(MyGraphics, RandomGenerator, PositionDistribution, RotationDistribution, MaterialColor, 1.0f);
		}

	private:
		Graphics& MyGraphics;
		std::mt19937 RandomGenerator {std::random_device{}()};
		std::uniform_real_distribution<float> PositionDistribution {-1.0f, 1.0f};
		std::uniform_real_distribution<float> RotationDistribution {0, Math::PI * 0.25f};
		std::uniform_real_distribution<float> Color {0.0f, 1.0f};
	};

	const Factory MyFactory {MyWindow.GetGraphics()};
	Drawables.reserve(DrawablesNum);
	std::generate_n(std::back_inserter(Drawables), DrawablesNum, MyFactory);
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
	MyPointLight.Bind(MyWindow.GetGraphics(), MyCamera.GetMatrix());

	for (const auto& Drawable : Drawables)
	{
		Drawable->Update(MyWindow.MyKeyboard.IsKeyPressed(VK_SPACE) ? 0.0f : DeltaTime);
		Drawable->Draw(MyWindow.GetGraphics());
	}
	MyPointLight.Draw(MyWindow.GetGraphics());

	if (ImGui::Begin("Simulation"))
	{
		ImGui::SliderFloat("Speed", &SpeedFactor, 0.0f, 4.0f);
		ImGui::Text("%d FPS", static_cast<int>(ImGui::GetIO().Framerate));
		ImGui::Text("Status : %s", MyWindow.MyKeyboard.IsKeyPressed(VK_SPACE) ? "PAUSED" : "RUNNING");
	}

	ImGui::End();

	MyCamera.ShowControlWindow();
	MyPointLight.ShowControlWindow();
	MyWindow.GetGraphics().EndFrame();
}
