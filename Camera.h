#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	[[nodiscard]] DirectX::XMMATRIX GetMatrix() const noexcept;
	void ShowControlWindow() noexcept;
	DirectX::XMFLOAT3 GetPosition() const
	{
		return {0.0f, 0.0f, -Z};
	}
	void Reset() noexcept;

private:
	float Z {20.0f};
	float Theta {0.0f};
	float Phi {0.0f};
	float Pitch {0.0f};
	float Yaw {0.0f};
	float Roll {0.0f};
};
