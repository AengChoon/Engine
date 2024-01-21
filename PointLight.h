#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight(Graphics& InGraphics, float InRadius = 0.5f);
	void ShowControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(const Graphics& InGraphics) const;
	void Bind(const Graphics& InGraphics, const DirectX::FXMMATRIX& InViewMatrix) const noexcept;

private:
	struct PointLightConstantBuffer
	{
		alignas(16) DirectX::XMFLOAT3 Position;
		alignas(16) DirectX::XMFLOAT3 AmbientColor;
		alignas(16) DirectX::XMFLOAT3 DiffuseColor;
		float DiffuseStrength;
		float QuadraticAttenuation;
		float LinearAttenuation;
		float ConstantAttenuation;
	};

private:
	PointLightConstantBuffer ConstantBufferData;
	mutable SolidSphere Mesh;
	mutable PixelConstantBuffer<PointLightConstantBuffer> ConstantBuffer;
};