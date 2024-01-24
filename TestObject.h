#pragma once
#include <random>
#include "DrawableBase.h"

template<class T>
class TestObject : public DrawableBase<T>
{
public:
	TestObject(const Graphics& InGraphics, std::mt19937& InRandomEngine,
	           std::uniform_real_distribution<float>& InPositionDistribution,
	           std::uniform_real_distribution<float>& InRotationDistribution)
		: DeltaX(InPositionDistribution(InRandomEngine)),
		  DeltaY(InPositionDistribution(InRandomEngine)),
		  DeltaZ(InPositionDistribution(InRandomEngine)),
		  DeltaRoll(InRotationDistribution(InRandomEngine)),
		  DeltaPitch(InRotationDistribution(InRandomEngine)),
		  DeltaYaw(InRotationDistribution(InRandomEngine))
	{}

	void Update(const float InDeltaTime) noexcept override
	{
		Roll += DeltaRoll * InDeltaTime;
		Pitch += DeltaPitch * InDeltaTime;
		Yaw += DeltaYaw * InDeltaTime;

		if (DirectX::XMVector3Length({X, Y, Z}).m128_f32[0] < 10.0f)
		{
			X += DeltaX * InDeltaTime;
			Y += DeltaY * InDeltaTime;
			Z += DeltaZ * InDeltaTime;
		}
	}

	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept override
	{
		return DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll) *
			   DirectX::XMMatrixTranslation(X, Y, Z);
	}

protected:
	float X {0.0f};
	float Y {0.0f};
	float Z {0.0f};
	float DeltaX;
	float DeltaY;
	float DeltaZ;

	float Roll {0.0f};
	float Pitch {0.0f};
	float Yaw {0.0f};
	float DeltaRoll;
	float DeltaPitch;
	float DeltaYaw;
};