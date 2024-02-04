#include "SolidSphere.h"
#include "Sphere.h"
#include "Bindables.h"

SolidSphere::SolidSphere(Graphics& InGraphics, float InRadius)
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
	};

	auto Model = Sphere::Make();
	Model.Transform(DirectX::XMMatrixScaling(InRadius, InRadius, InRadius));

	Bind(std::make_shared<VertexBuffer>(InGraphics, Model.Vertices));

	Bind(std::make_shared<IndexBuffer>(InGraphics, Model.Indices));

	auto ModelVertexShader = std::make_shared<VertexShader>(InGraphics, L"SolidVS.cso");
	auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
	Bind(std::move( ModelVertexShader ));

	Bind(std::make_shared<PixelShader>(InGraphics, L"SolidPS.cso"));

	struct PSColorConstants
	{
		DirectX::XMFLOAT3 color {1.0f,1.0f,1.0f};
		float padding;
	} ModelColorConstants;

	Bind(std::make_shared<PixelConstantBuffer<PSColorConstants>>(InGraphics, ModelColorConstants));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ModelInputElementDescs =
	{
		{
			"Position",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	Bind(std::make_shared<InputLayout>(InGraphics, ModelInputElementDescs, ModelVertexShaderBlob));

	Bind(std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	Bind(std::make_shared<TransformConstantBuffer>(InGraphics, *this));
}

DirectX::XMMATRIX SolidSphere::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z);
}

void SolidSphere::SetPosition(const DirectX::XMFLOAT3& InPosition) noexcept
{
	Position = InPosition;
}
