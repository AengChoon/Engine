#include "TransformConstantBuffer.h"
#include "Camera.h"
#include "Drawable.h"

TransformConstantBuffer::TransformConstantBuffer(const Graphics& InGraphics, const Drawable& InParent, const UINT InSlot)
	: Parent(InParent)
{
	if (!MyVertexConstantBuffer)
	{
		MyVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(InGraphics, InSlot);
	}
}

void TransformConstantBuffer::Bind(const Graphics& InGraphics) noexcept
{
	const auto WorldTransformMatrix = Parent.get().GetTransformMatrix();

	const Transforms CurrentTransforms
	{
		DirectX::XMMatrixTranspose(WorldTransformMatrix),
		DirectX::XMMatrixTranspose
		(
			WorldTransformMatrix *
			InGraphics.GetCamera().GetMatrix() *
			InGraphics.GetProjectionMatrix()
		)
	};

	MyVertexConstantBuffer->Update(InGraphics, CurrentTransforms);
	MyVertexConstantBuffer->Bind(InGraphics);
}
