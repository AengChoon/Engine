#include "TransformConstantBuffer.h"
#include "Drawable.h"

TransformConstantBuffer::TransformConstantBuffer(const Graphics& InGraphics, const Drawable& InParent)
	: Parent(InParent)
{
	if (!MyVertexConstantBuffer)
	{
		MyVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(InGraphics);
	}
}

void TransformConstantBuffer::Bind(const Graphics& InGraphics) noexcept
{
	const auto ModelTransformMatrix = Parent.GetTransformMatrix();

	const Transforms ModelTransforms
	{
		DirectX::XMMatrixTranspose(ModelTransformMatrix),
		DirectX::XMMatrixTranspose
		(
			ModelTransformMatrix *
			InGraphics.GetCameraMatrix() *
			InGraphics.GetProjectionMatrix()
		)
	};

	MyVertexConstantBuffer->Update(InGraphics, ModelTransforms);
	MyVertexConstantBuffer->Bind(InGraphics);
}
