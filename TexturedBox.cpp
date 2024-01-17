#include "TexturedBox.h"
#include "Bindables.h"
#include "Cube.h"
#include "Surface.h"

TexturedBox::TexturedBox(Graphics& InGraphics, std::mt19937& InRandomGenerator,
                         std::uniform_real_distribution<float>& InA, std::uniform_real_distribution<float>& InB,
                         std::uniform_real_distribution<float>& InC, std::uniform_real_distribution<float>& InD)
	: X(InD(InRandomGenerator)),
	  Theta(InA(InRandomGenerator)), Phi(InA(InRandomGenerator)), Chi(InA(InRandomGenerator)),
	  DeltaRoll(InB(InRandomGenerator)), DeltaPitch(InB(InRandomGenerator)), DeltaYaw(InB(InRandomGenerator)),
	  DeltaTheta(InC(InRandomGenerator)), DeltaPhi(InC(InRandomGenerator)), DeltaChi(InC(InRandomGenerator))
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 Position;

			struct 
			{
				float U;
				float V;
			} Texture;
		};

		const auto Model = Cube::MakeTextured<Vertex>();

		AddStaticBindable(std::make_unique<VertexBuffer>(InGraphics, Model.Vertices));
		AddStaticBindable(std::make_unique<Texture>(InGraphics, Surface::FromFile("Images\\cube.png")));
		AddStaticBindable(std::make_unique<Sampler>(InGraphics));

		auto BoxVertexShader = std::make_unique<VertexShader>(InGraphics, L"TextureVS.cso");
		auto BoxVertexShaderBlob = BoxVertexShader->GetByteCode();
		AddStaticBindable(std::move(BoxVertexShader));

		AddStaticBindable(std::make_unique<PixelShader>(InGraphics, L"TexturePS.cso"));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(InGraphics, Model.Indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TextureCoordinate", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		AddStaticBindable(std::make_unique<InputLayout>(InGraphics, InputElementDescs, BoxVertexShaderBlob));
		AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexBufferFromStaticBindables();
	}

	AddInstanceBindable(std::make_unique<TransformConstantBuffer>(InGraphics, *this));
}

void TexturedBox::Update(float InDeltaTime) noexcept
{
	Roll += DeltaRoll * InDeltaTime;
	Pitch += DeltaPitch * InDeltaTime;
	Yaw += DeltaYaw * InDeltaTime;
	Theta += DeltaTheta * InDeltaTime;
	Phi += DeltaPhi * InDeltaTime;
	Chi += DeltaChi * InDeltaTime;
}

DirectX::XMMATRIX TexturedBox::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll) *
		   DirectX::XMMatrixTranslation(X, 0.0f, 0.0f) *
		   DirectX::XMMatrixRotationRollPitchYaw(Theta, Phi, Chi) *
		   DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
