#include "Camera.h"
#include <algorithm>
#include "EngineMath.h"
#include "imgui/imgui.h"

Camera::Camera() noexcept
{
	Reset();
}

void Camera::Reset() noexcept
{
	Position = {0.0f, 7.5f, -18.0f};
	Pitch = 0.0f;
	Yaw = 0.0f;
}

void Camera::Rotate(const float InDeltaX, const float InDeltaY) noexcept
{
	Yaw = Math::WrapAngle(Yaw + InDeltaX * RotationSpeed);
	Pitch = std::clamp(Pitch + InDeltaY * RotationSpeed, 0.995f * -Math::PI / 2.0f, 0.995f * Math::PI / 2.0f);
}

void Camera::Translate(DirectX::XMFLOAT3 InTranslation) noexcept
{
	const auto RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, 0.0f);
	const auto ScaleMatrix = DirectX::XMMatrixScaling(TranslationSpeed, TranslationSpeed, TranslationSpeed);
	const auto TranslationResult = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&InTranslation), RotationMatrix * ScaleMatrix);
	DirectX::XMStoreFloat3(&InTranslation, TranslationResult);

	Position.x += InTranslation.x;
	Position.y += InTranslation.y;
	Position.z += InTranslation.z;
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto ForwardBaseVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const auto UpBaseVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const auto WorldRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, 0.0f);
	const auto LookVector = DirectX::XMVector3Transform(ForwardBaseVector, WorldRotationMatrix);
	const auto CameraPosition = DirectX::XMLoadFloat3(&Position);
	const auto LookPosition = DirectX::XMVectorAdd(CameraPosition, LookVector);

	return DirectX::XMMatrixLookAtLH(CameraPosition, LookPosition, UpBaseVector);
}

DirectX::XMFLOAT3 Camera::GetPosition() const
{
	return Position;
}

void Camera::ShowControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &Position.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &Position.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &Position.z, -80.0f, 80.0f, "%.1f");

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Pitch", &Pitch, 0.995f * -90.0f, 0.995f * 90.0f);
		ImGui::SliderAngle("Yaw", &Yaw, -180.0f, 180.0f);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}

	ImGui::End();
}