#include "SolidSphere.h"
#include "Sphere.h"
#include "Bindables.h"

SolidSphere::SolidSphere(Graphics& InGraphics, const float InRadius)
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
	};

	auto Model = Sphere::Make();
	Model.Transform(DirectX::XMMatrixScaling(InRadius, InRadius, InRadius));

	const auto GeometryTag = "$sphere." + std::to_string(InRadius);

	Bind(VertexBuffer::Resolve(InGraphics, GeometryTag, Model.Vertices));

	Bind(IndexBuffer::Resolve(InGraphics, GeometryTag, Model.Indices));

	auto ModelVertexShader = VertexShader::Resolve(InGraphics, "SolidVS.cso");
	auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
	Bind(std::move(ModelVertexShader));

	Bind(PixelShader::Resolve(InGraphics, "SolidPS.cso"));

	struct PSColorConstants
	{
		DirectX::XMFLOAT3 color {1.0f,1.0f,1.0f};
		float padding;
	} ModelColorConstants;

	Bind(PixelConstantBuffer<PSColorConstants>::Resolve(InGraphics, ModelColorConstants));

	Bind(InputLayout::Resolve(InGraphics, Model.Vertices.GetLayout(), ModelVertexShaderBlob));

	Bind(Topology::Resolve(InGraphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	Bind(std::make_shared<TransformConstantBuffer>(InGraphics, *this, TransformConstantBuffer::Target::Vertex));
}

DirectX::XMMATRIX SolidSphere::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z);
}

void SolidSphere::SetPosition(const DirectX::XMFLOAT3& InPosition) noexcept
{
	Position = InPosition;
}
