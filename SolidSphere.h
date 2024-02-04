#pragma once
#include "Drawable.h"

class SolidSphere : public Drawable
{
public:
	SolidSphere(Graphics& InGraphics, float InRadius);

	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept override;
	void SetPosition(const DirectX::XMFLOAT3& InPosition) noexcept;

private:
	DirectX::XMFLOAT3 Position {1.0f,1.0f,1.0f};
};
