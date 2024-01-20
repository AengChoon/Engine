#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& InGraphics, float InRadius)
	: Mesh(InGraphics, InRadius), ConstantBuffer(InGraphics)
{
}

void PointLight::ShowControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &Position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &Position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &Position.z, -60.0f, 60.0f, "%.1f");

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}

	ImGui::End();
}

void PointLight::Reset() noexcept
{
	Position = {0.0f, 0.0f, 0.0f};
}

void PointLight::Draw(const Graphics& InGraphics) const
{
	Mesh.SetPosition(Position);
	Mesh.Draw(InGraphics);
}

void PointLight::Bind(const Graphics& InGraphics) const noexcept
{
	ConstantBuffer.Update(InGraphics, PointLightConstantBuffer{Position});
	ConstantBuffer.Bind(InGraphics);
}
