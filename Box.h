#pragma once
#include <random>
#include "DrawableBase.h"

class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& InGraphics, std::mt19937& InRandomGenerator, std::uniform_real_distribution<float>& InA,
	    std::uniform_real_distribution<float>& InB, std::uniform_real_distribution<float>& InC,
	    std::uniform_real_distribution<float>& InD, std::uniform_real_distribution<float>& InE,
		DirectX::XMFLOAT3 InMaterialColor);

	void Update(float InDeltaTime) noexcept override;
	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
	float X;
	float Roll = 0.0f;
	float Pitch = 0.0f;
	float Yaw = 0.0f;
	float Theta;
	float Phi;
	float Chi;
	float DeltaRoll;
	float DeltaPitch;
	float DeltaYaw;
	float DeltaTheta;
	float DeltaPhi;
	float DeltaChi;
	DirectX::XMFLOAT3X3 ModelTransform;
};
