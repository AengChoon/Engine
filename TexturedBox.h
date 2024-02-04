#pragma once
#include "Drawable.h"

class TexturedBox : public Drawable
{
public:
	TexturedBox(const Graphics& InGraphics);

	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
	float X {0.0f};
	float Y {0.0f};
	float Z {0.0f};
};
