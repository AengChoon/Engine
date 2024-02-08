#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Bindable.h"
#include "ConstantBuffers.h"

class Drawable;

class TransformConstantBuffer : public Bindable
{
public:
	enum class Target
	{
		Vertex,
		Pixel
	};

private:
	struct Transforms
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX WorldViewProjection;
	};

public:
	TransformConstantBuffer(const Graphics& InGraphics, const Drawable& InParent, Target InType, UINT InSlot = 0u);

	void Bind(const Graphics& InGraphics) noexcept override;
	Transforms GetTransforms(const Graphics& InGraphics) const noexcept;

private:
	void BindImpl(const Graphics& InGraphics, const Transforms& InTransforms) const noexcept;


private:
	static inline std::unique_ptr<VertexConstantBuffer<Transforms>> MyVertexConstantBuffer;
	static inline std::unique_ptr<PixelConstantBuffer<Transforms>> MyPixelConstantBuffer;
	std::reference_wrapper<const Drawable> Parent;
	Target Type;
};
