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
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX WorldViewProjection;
	};

public:
	TransformConstantBuffer(const Graphics& InGraphics, const Drawable& InParent, UINT InSlot = 0u);

	void Bind(const Graphics& InGraphics) noexcept override;

private:
	static inline std::unique_ptr<VertexConstantBuffer<Transforms>> MyVertexConstantBuffer;
	std::reference_wrapper<const Drawable> Parent;
};
