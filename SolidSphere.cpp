#include "SolidSphere.h"
#include "Sphere.h"
#include "Bindables.h"

SolidSphere::SolidSphere(Graphics& InGraphics, float InRadius)
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 Position;
		};

		auto Model = Sphere::Make<Vertex>();
		Model.Transform(DirectX::XMMatrixScaling(InRadius, InRadius, InRadius));
		AddStaticBindable(std::make_unique<VertexBuffer>(InGraphics, Model.Vertices));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(InGraphics, Model.Indices));

		auto pvs = std::make_unique<VertexShader>(InGraphics, L"SolidVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBindable(std::move( pvs ));

		AddStaticBindable(std::make_unique<PixelShader>(InGraphics, L"SolidPS.cso"));

		struct PSColorConstant
		{
			DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;
		} colorConst;

		AddStaticBindable(std::make_unique<PixelConstantBuffer<PSColorConstant>>(InGraphics, colorConst));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		AddStaticBindable(std::make_unique<InputLayout>(InGraphics, ied, pvsbc));

		AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexBufferFromStaticBindables();
	}

	AddInstanceBindable(std::make_unique<TransformConstantBuffer>(InGraphics, *this));
}

void SolidSphere::Update(float InDeltaTime) noexcept
{
}

DirectX::XMMATRIX SolidSphere::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z);
}

void SolidSphere::SetPosition(const DirectX::XMFLOAT3& InPosition) noexcept
{
	Position = InPosition;
}
