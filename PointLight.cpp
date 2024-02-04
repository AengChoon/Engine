#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& InGraphics, float InRadius)
	: Mesh(InGraphics, InRadius), ConstantBuffer(InGraphics)
{
	Reset();
}

void PointLight::ShowControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &ConstantBufferData.Position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &ConstantBufferData.Position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &ConstantBufferData.Position.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Diffuse");
		ImGui::SliderFloat("Strength", &ConstantBufferData.DiffuseStrength, -60.0f, 60.0f, "%.1f");
		ImGui::ColorEdit3("Color", &ConstantBufferData.DiffuseColor.x);

		ImGui::Text("Ambient");
		ImGui::ColorEdit3("Color", &ConstantBufferData.AmbientColor.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Quadratic", &ConstantBufferData.QuadraticAttenuation, 0.0f, 10.0f, "%.2f");
		ImGui::SliderFloat("Linear", &ConstantBufferData.LinearAttenuation, 0.0001f, 4.0f, "%.4f");
		ImGui::SliderFloat("Constant", &ConstantBufferData.ConstantAttenuation, 0.0000001f, 10.0f, "%.7f");

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}

	ImGui::End();
}

void PointLight::Reset() noexcept
{
	ConstantBufferData =
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
	Mesh.SetPosition(ConstantBufferData.Position);
	Mesh.Draw(InGraphics);
}

void PointLight::Bind(const Graphics& InGraphics, const DirectX::FXMMATRIX& InViewMatrix) const noexcept
{	
	ConstantBuffer.Update(InGraphics, ConstantBufferData);
	ConstantBuffer.Bind(InGraphics);
}
