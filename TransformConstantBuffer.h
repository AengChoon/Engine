#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Bindable.h"
#include "ConstantBuffers.h"

class Drawable;

class TransformConstantBuffer : public Bindable
{
	struct Transforms
	{
		DirectX::XMMATRIX Model;
		DirectX::XMMATRIX ModelViewProjection;
	};

public:
	TransformConstantBuffer(const Graphics& InGraphics, const Drawable& InParent);

	void Bind(const Graphics& InGraphics) noexcept override;

private:
	static inline std::unique_ptr<VertexConstantBuffer<Transforms>> MyVertexConstantBuffer;
	const Drawable& Parent;
};
