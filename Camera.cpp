#include "Camera.h"
#include "imgui/imgui.h"

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto WorldTranslation = DirectX::XMMatrixTranslation(0.0f, 0.0f, -Z);
	const auto WorldRotation = DirectX::XMMatrixRotationRollPitchYaw(Phi, -Theta, 0.0f);
	const auto WorldTransform =  WorldTranslation * WorldRotation;
	const auto LocalTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

	const auto Position
	{
	 	DirectX::XMVector3Transform
			(
				DirectX::XMVectorSet(0.0f, 0.0f, -Z, 0.0f),
				DirectX::XMMatrixRotationRollPitchYaw(Phi, -Theta, 0.0f)
			)
	};

	return DirectX::XMMatrixLookAtLH
	(
		Position,
		DirectX::XMVectorZero(),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * DirectX::XMMatrixRotationRollPitchYaw(Pitch, -Yaw, Roll);
}

void Camera::ShowControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("Z", &Z, 0.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &Theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &Phi, -89.0f, 89.0f);

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &Roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &Pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &Yaw, -180.0f, 180.0f);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}

	ImGui::End();
}

void Camera::Reset() noexcept
{
	Z = 20.0f;
	Theta = 0.0f;
	Phi = 0.0f;
	Pitch = 0.0f;
	Yaw = 0.0f;
	Roll = 0.0f;
}
