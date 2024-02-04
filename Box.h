#pragma once
#include "Drawable.h"

class Box : public Drawable
{
public:
	Box(const Graphics& InGraphics, DirectX::XMFLOAT3 InMaterialColor);
	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
	float X {0.0f};
	float Y {0.0f};
	float Z {0.0f};
};
