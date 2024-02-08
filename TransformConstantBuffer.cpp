#include "TransformConstantBuffer.h"
#include "Camera.h"
#include "Drawable.h"

TransformConstantBuffer::TransformConstantBuffer(const Graphics& InGraphics, const Drawable& InParent,
                                                 const Target InType, UINT InSlot)
	: Parent(InParent)
	, Type(InType)
{
	if (!MyVertexConstantBuffer)
	{
		MyVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(InGraphics, InSlot);
	}

	if (!MyPixelConstantBuffer)
	{
		MyPixelConstantBuffer = std::make_unique<PixelConstantBuffer<Transforms>>(InGraphics, InSlot);
	}
}

void TransformConstantBuffer::Bind(const Graphics& InGraphics) noexcept
{
	BindImpl(InGraphics, GetTransforms(InGraphics));
}

void TransformConstantBuffer::BindImpl(const Graphics& InGraphics, const Transforms& InTransforms) const noexcept
{
	switch (Type)
	{
	case Target::Vertex:
		{
			MyVertexConstantBuffer->Update(InGraphics, InTransforms);
			MyVertexConstantBuffer->Bind(InGraphics);
		}
		break;
	case Target::Pixel:
		{
			MyPixelConstantBuffer->Update(InGraphics, InTransforms);
			MyPixelConstantBuffer->Bind(InGraphics);
		}
		break;
	}
}

TransformConstantBuffer::Transforms TransformConstantBuffer::GetTransforms(const Graphics& InGraphics) const noexcept
{
	const auto WorldTransformMatrix = Parent.get().GetTransformMatrix();

	return
	{
		DirectX::XMMatrixTranspose(WorldTransformMatrix),
		DirectX::XMMatrixTranspose
		(
			WorldTransformMatrix *
			InGraphics.GetCamera().GetMatrix() *
			InGraphics.GetProjectionMatrix()
		)
	};
}


