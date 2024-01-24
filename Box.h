#pragma once
#include <random>
#include "TestObject.h"

class Box : public TestObject<Box>
{
public:
	Box(const Graphics& InGraphics, std::mt19937& InRandomEngine,
		std::uniform_real_distribution<float>& InPositionDistribution,
	    std::uniform_real_distribution<float>& InRotationDistribution,
		DirectX::XMFLOAT3 InMaterialColor);

	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
	DirectX::XMFLOAT3X3 ModelTransform;
};
