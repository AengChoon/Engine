#pragma once
#include "TestObject.h"

class AssetTest : public TestObject<AssetTest>
{
public:
	AssetTest(const Graphics& InGraphics, std::mt19937& InRandomEngine,
			  std::uniform_real_distribution<float>& InPositionDistribution,
			  std::uniform_real_distribution<float>& InRotationDistribution,
			  DirectX::XMFLOAT3 InMaterialColor,
			  float InScale);
};