#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& InGraphics, const float InRadius)
	: Mesh(InGraphics, InRadius), ConstantBuffer(InGraphics)
{
	Reset();
}

void PointLight::ShowControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &Constants.Position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &Constants.Position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &Constants.Position.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Diffuse");
		ImGui::SliderFloat("Strength", &Constants.DiffuseStrength, -60.0f, 60.0f, "%.1f");
		ImGui::ColorEdit3("Color", &Constants.DiffuseColor.x);

		ImGui::Text("Ambient");
		ImGui::ColorEdit3("Color", &Constants.AmbientColor.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Quadratic", &Constants.QuadraticAttenuation, 0.0f, 10.0f, "%.2f");
		ImGui::SliderFloat("Linear", &Constants.LinearAttenuation, 0.0001f, 4.0f, "%.4f");
		ImGui::SliderFloat("Constant", &Constants.ConstantAttenuation, 0.0000001f, 10.0f, "%.7f");

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}

	ImGui::End();
}

void PointLight::Reset() noexcept
{
	Constants =
	{
		{1.5f, 14.0f, -4.5f},
		{0.05f, 0.05f, 0.05f},
		{1.0f, 1.0f, 1.0f},
		1.0f,
		0.0075f,
		0.045f,
		1.0f
	};
}

void PointLight::Draw(const Graphics& InGraphics) const
{
	Mesh.SetPosition(Constants.Position);
	Mesh.Draw(InGraphics);
}

void PointLight::Bind(const Graphics& InGraphics, const DirectX::FXMMATRIX& InViewMatrix) const noexcept
{	
	ConstantBuffer.Update(InGraphics, Constants);
	ConstantBuffer.Bind(InGraphics);
}