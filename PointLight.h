#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

struct PointLightConstants
{
	alignas(16) DirectX::XMFLOAT3 Position;
	alignas(16) DirectX::XMFLOAT3 AmbientColor;
	alignas(16) DirectX::XMFLOAT3 DiffuseColor;
	float DiffuseStrength;
	float QuadraticAttenuation;
	float LinearAttenuation;
	float ConstantAttenuation;
};

class PointLight
{
public:
	PointLight(Graphics& InGraphics, float InRadius = 0.5f);

	void ShowControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(const Graphics& InGraphics) const;
	void Bind(const Graphics& InGraphics, const DirectX::FXMMATRIX& InViewMatrix) const noexcept;

private:
	PointLightConstants Constants;
	mutable SolidSphere Mesh;
	mutable PixelConstantBuffer<PointLightConstants> ConstantBuffer;
};