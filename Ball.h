#pragma once
#include <random>
#include "DrawableBase.h"

class Ball : public DrawableBase<Ball>
{
public:
	Ball(Graphics& InGraphics, std::mt19937& InRandomGenerator, std::uniform_real_distribution<float>& InA,
	    std::uniform_real_distribution<float>& InB, std::uniform_real_distribution<float>& InC,
	    std::uniform_real_distribution<float>& InD, std::uniform_int_distribution<int>& InLongitude,
		std::uniform_int_distribution<int>& InLatitude);

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
};
