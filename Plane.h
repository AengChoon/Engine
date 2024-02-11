#pragma once
#include "Drawable.h"

class Plane : public Drawable
{
public:
	Plane(const Graphics& InGraphics, float InSize);
	void SetPosition(DirectX::XMFLOAT3 InPosition) noexcept;
	void SetRotation(float InRoll, float InPitch, float InYaw) noexcept;
	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
	DirectX::XMFLOAT3 Position {1.0f, 1.0f, 1.0f};
	float Roll {0.0f};
	float Pitch {0.0f};
	float Yaw {0.0f};
};
