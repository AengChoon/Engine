#include "TransformConstantBuffer.h"
#include "Drawable.h"

TransformConstantBuffer::TransformConstantBuffer(const Graphics& InGraphics, const Drawable& InParent)
	: Parent(InParent)
{
	if (!MyVertexConstantBuffer)
	{
		MyVertexConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(InGraphics);
	}
}

void TransformConstantBuffer::Bind(const Graphics& InGraphics) noexcept
{
	MyVertexConstantBuffer->Update
	(
		InGraphics,
		DirectX::XMMatrixTranspose(Parent.GetTransformMatrix() * InGraphics.GetProjection())
	);

	MyVertexConstantBuffer->Bind(InGraphics);
}
