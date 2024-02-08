#include "TexturedBox.h"
#include "Bindables.h"
#include "Cube.h"
#include "Surface.h"

TexturedBox::TexturedBox(const Graphics& InGraphics)
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;

		struct 
		{
			float U;
			float V;
		} TextureCoordinates;
	};

	const auto Model = Cube::MakeTextured();

	Bind(std::make_shared<VertexBuffer>(InGraphics, Model.Vertices));

	Bind(std::make_shared<IndexBuffer>(InGraphics, Model.Indices));

	Bind(std::make_shared<Texture>(InGraphics, "Images\\cube.png"));

	Bind(std::make_shared<Sampler>(InGraphics));

	auto ModelVertexShader = std::make_shared<VertexShader>(InGraphics, "TextureVS.cso");
	auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
	Bind(std::move(ModelVertexShader));

	Bind(std::make_shared<PixelShader>(InGraphics, "TexturePS.cso"));

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
			"TextureCoordinate",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	// Bind(std::make_shared<InputLayout>(InGraphics, ModelInputElementDescs, ModelVertexShaderBlob));

	Bind(std::make_shared<Topology>(InGraphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	Bind(std::make_shared<TransformConstantBuffer>(InGraphics, *this, TransformConstantBuffer::Target::Vertex));
}

DirectX::XMMATRIX TexturedBox::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixTranslation(X, Y, Z);
}
