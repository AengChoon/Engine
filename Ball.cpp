#include "Ball.h"

#include "ConstantBuffers.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "Sphere.h"
#include "Topology.h"
#include "TransformConstantBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

Ball::Ball(Graphics& InGraphics, std::mt19937& InRandomGenerator, std::uniform_real_distribution<float>& InA,
           std::uniform_real_distribution<float>& InB, std::uniform_real_distribution<float>& InC,
           std::uniform_real_distribution<float>& InD, std::uniform_int_distribution<int>& InLongitude,
           std::uniform_int_distribution<int>& InLatitude)
	: X(InD(InRandomGenerator)),
	  Theta(InA(InRandomGenerator)), Phi(InA(InRandomGenerator)), Chi(InA(InRandomGenerator)),
	  DeltaRoll(InB(InRandomGenerator)), DeltaPitch(InB(InRandomGenerator)), DeltaYaw(InB(InRandomGenerator)),
	  DeltaTheta(InC(InRandomGenerator)), DeltaPhi(InC(InRandomGenerator)), DeltaChi(InC(InRandomGenerator))
{
	if (!IsStaticInitialized())
	{
		auto BallVertexShader = std::make_unique<VertexShader>(InGraphics, L"ColorIndexVS.cso");
		auto BallVertexShaderBlob = BallVertexShader->GetByteCode();
		AddStaticBindable(std::move(BallVertexShader));

		AddStaticBindable(std::make_unique<PixelShader>(InGraphics, L"ColorIndexPS.cso"));

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

		constexpr PixelShaderConstants BallPixelShaderConstants =
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

		AddStaticBindable(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(InGraphics, BallPixelShaderConstants));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		AddStaticBindable(std::make_unique<InputLayout>(InGraphics, InputElementDescs, BallVertexShaderBlob));
		AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
	};

	auto Model = Sphere::MakeTessellated<Vertex>(InLatitude(InRandomGenerator), InLongitude(InRandomGenerator));
	Model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.2f));

	AddInstanceBindable(std::make_unique<VertexBuffer>(InGraphics, Model.Vertices));
	AddInstanceIndexBuffer(std::make_unique<IndexBuffer>(InGraphics, Model.Indices));
	AddInstanceBindable(std::make_unique<TransformConstantBuffer>(InGraphics, *this));
}

void Ball::Update(float InDeltaTime) noexcept
{
	Roll += DeltaRoll * InDeltaTime;
	Pitch += DeltaPitch * InDeltaTime;
	Yaw += DeltaYaw * InDeltaTime;
	Theta += DeltaTheta * InDeltaTime;
	Phi += DeltaPhi * InDeltaTime;
	Chi += DeltaChi * InDeltaTime;
}

DirectX::XMMATRIX Ball::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll) *
		   DirectX::XMMatrixTranslation(X, 0.0f, 0.0f) *
		   DirectX::XMMatrixRotationRollPitchYaw(Theta, Phi, Chi) *
		   DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
