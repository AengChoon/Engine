#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Bindable.h"
#include "ConstantBuffers.h"

class Drawable;

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(const Graphics& InGraphics, const Drawable& InParent);

	void Bind(const Graphics& InGraphics) noexcept override;

private:
	static inline std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> MyVertexConstantBuffer;
	const Drawable& Parent;
};
