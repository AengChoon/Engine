#include "Box.h"
#include "Bindables.h"
#include "Cube.h"

Box::Box(Graphics& InGraphics, std::mt19937& InRandomGenerator, 
         std::uniform_real_distribution<float>& InA, std::uniform_real_distribution<float>& InB,
         std::uniform_real_distribution<float>& InC, std::uniform_real_distribution<float>& InD,
		 std::uniform_real_distribution<float>& InE)
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
		};

		const auto Model = Cube::Make<Vertex>();

		AddStaticBindable(std::make_unique<VertexBuffer>(InGraphics, Model.Vertices));

		auto BoxVertexShader = std::make_unique<VertexShader>(InGraphics, L"ColorIndexVS.cso");
		auto BoxVertexShaderBlob = BoxVertexShader->GetByteCode();
		AddStaticBindable(std::move(BoxVertexShader));

		AddStaticBindable(std::make_unique<PixelShader>(InGraphics, L"ColorIndexPS.cso"));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(InGraphics, Model.Indices));

		struct PixelShaderConstants
		{
			struct
			{
				float R;
				float G;
				float B;
				float A;
			} FaceColors[8];
		};

		constexpr PixelShaderConstants BoxPixelShaderConstants =
		{
			{
				{1.0f, 1.0f, 1.0f},
				{ 1.0f,0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{1.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{1.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 1.0f},
				{0.0f, 0.0f, 0.0f}
			}
		};

		AddStaticBindable(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(InGraphics, BoxPixelShaderConstants));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		AddStaticBindable(std::make_unique<InputLayout>(InGraphics, InputElementDescs, BoxVertexShaderBlob));
		AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexBufferFromStaticBindables();
	}

	AddInstanceBindable(std::make_unique<TransformConstantBuffer>(InGraphics, *this));
	DirectX::XMStoreFloat3x3(&ModelTransform, DirectX::XMMatrixScaling(1.0f, 1.0f, InE(InRandomGenerator)));
}

void Box::Update(float InDeltaTime) noexcept
{
	Roll += DeltaRoll * InDeltaTime;
	Pitch += DeltaPitch * InDeltaTime;
	Yaw += DeltaYaw * InDeltaTime;
	Theta += DeltaTheta * InDeltaTime;
	Phi += DeltaPhi * InDeltaTime;
	Chi += DeltaChi * InDeltaTime;
}

DirectX::XMMATRIX Box::GetTransformMatrix() const noexcept
{
	return DirectX::XMLoadFloat3x3(&ModelTransform) *
		   DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll) *
		   DirectX::XMMatrixTranslation(X, 0.0f, 0.0f) *
		   DirectX::XMMatrixRotationRollPitchYaw(Theta, Phi, Chi) *
		   DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
