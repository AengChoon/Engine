#pragma once
#include "DrawableBase.h"

class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere(Graphics& InGraphics, float InRadius);

	void Update(float InDeltaTime) noexcept override;
	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

	void SetPosition(const DirectX::XMFLOAT3& InPosition) noexcept;

private:
	DirectX::XMFLOAT3 Position {1.0f,1.0f,1.0f};
};