#include "TransformConstantBuffer.h"
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
	const auto ModelViewMatrix = Parent.GetTransformMatrix() * InGraphics.GetCameraMatrix();

	const Transforms ModelTransforms
	{
		DirectX::XMMatrixTranspose(ModelViewMatrix),
		DirectX::XMMatrixTranspose
		(
			ModelViewMatrix *
			InGraphics.GetProjectionMatrix()
		)
	};

	MyVertexConstantBuffer->Update(InGraphics, ModelTransforms);
	MyVertexConstantBuffer->Bind(InGraphics);
}
