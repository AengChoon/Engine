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
	void Bind(const Graphics& InGraphics) const noexcept;

private:
	struct PointLightConstantBuffer
	{
		DirectX::XMFLOAT3 Position;
		float Padding;
	};

private:
	DirectX::XMFLOAT3 Position = { 0.0f,0.0f,0.0f };;
	mutable SolidSphere Mesh;
	mutable PixelConstantBuffer<PointLightConstantBuffer> ConstantBuffer;
};