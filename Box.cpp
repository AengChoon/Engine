#include "Box.h"
#include "Bindables.h"
#include "Camera.h"
#include "Cube.h"

Box::Box(const Graphics& InGraphics, const DirectX::XMFLOAT3 InMaterialColor)
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
	};

	auto Model = Cube::MakeIndependent();
	Model.SetNormalsIndependentFlat();

	Bind(std::make_shared<VertexBuffer>(InGraphics, Model.Vertices));

	Bind(std::make_shared<IndexBuffer>(InGraphics, Model.Indices));

	auto ModelVertexShader = std::make_shared<VertexShader>(InGraphics, "PhongVS.cso");
	auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
	Bind(std::move(ModelVertexShader));

	Bind(std::make_shared<PixelShader>(InGraphics, "PhongPS.cso"));

	struct PSMaterialConstants
	{
		alignas(16) DirectX::XMFLOAT3 Color;
		float SpecularIntensity = 0.6f;
		float SpecularPower = 10.0f;
		float Padding[2];
	} ModelMaterialConstants;
	ModelMaterialConstants.Color = InMaterialColor;

	struct PSCameraConstants
	{
		alignas(16) DirectX::XMFLOAT3 Position;
	} ModelCameraConstants;

	Bind(std::make_shared<PixelConstantBuffer<PSMaterialConstants>>(InGraphics, ModelMaterialConstants, 1u));
	Bind(std::make_shared<PixelConstantBuffer<PSCameraConstants>>(InGraphics, ModelCameraConstants, 2u));

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
		},
		{
			"Normal",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	// Bind(std::make_shared<InputLayout>(InGraphics, ModelInputElementDescs, ModelVertexShaderBlob));

	Bind(std::make_shared<Topology>(InGraphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	Bind(std::make_shared<TransformConstantBuffer>(InGraphics, *this));
}

DirectX::XMMATRIX Box::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixTranslation(X, Y, Z);
}
