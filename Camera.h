#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera() noexcept;

	void ShowControlWindow() noexcept;
	void Reset() noexcept;
	void Rotate(float InDeltaX, float InDeltaY) noexcept;
	void Translate(DirectX::XMFLOAT3 InTranslation) noexcept;
	[[nodiscard]] DirectX::XMMATRIX GetMatrix() const noexcept;
	[[nodiscard]] DirectX::XMFLOAT3 GetPosition() const;

private:
	static constexpr float TranslationSpeed = 12.0f;
	static constexpr float RotationSpeed = 0.004f;

	DirectX::XMFLOAT3 Position;
	float Pitch {0.0f};
	float Yaw {0.0f};
};