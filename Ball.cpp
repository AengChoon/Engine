#include "Ball.h"
#include "Sphere.h"
#include "Bindables.h"
#include "TransformConstantBuffer.h"

Ball::Ball(Graphics& InGraphics, std::mt19937& InRandomGenerator, std::uniform_real_distribution<float>& InA,
           std::uniform_real_distribution<float>& InB, std::uniform_real_distribution<float>& InC,
           std::uniform_real_distribution<float>& InD, std::uniform_int_distribution<int>& InLongitude,
           std::uniform_int_distribution<int>& InLatitude)
	: X(InD(InRandomGenerator)),
	  Theta(InA(InRandomGenerator)), Phi(InA(InRandomGenerator)), Chi(InA(InRandomGenerator)),
	  DeltaRoll(InB(InRandomGenerator)), DeltaPitch(InB(InRandomGenerator)), DeltaYaw(InB(InRandomGenerator)),
	  DeltaTheta(InC(InRandomGenerator)), DeltaPhi(InC(InRandomGenerator)), DeltaChi(InC(InRandomGenerator))
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
	};

	auto Model = Sphere::Make();
	Model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.2f));

	Bind(std::make_shared<VertexBuffer>(InGraphics, Model.Vertices));

	Bind(std::make_shared<IndexBuffer>(InGraphics, Model.Indices));

	auto ModelVertexShader = std::make_shared<VertexShader>(InGraphics, "ColorIndexVS.cso");
	auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
	Bind(std::move(ModelVertexShader));

	Bind(std::make_shared<PixelShader>(InGraphics, "ColorIndexPS.cso"));

	struct PSColorConstants
	{
		struct
		{
			float R;
			float G;
			float B;
			float A;
		} FaceColors[8];
	};

	constexpr PSColorConstants ModelColorConstants =
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

	// Bind(std::make_shared<InputLayout>(InGraphics, ModelInputElementDescs, ModelVertexShaderBlob));

	Bind(std::make_shared<Topology>(InGraphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	Bind(std::make_shared<TransformConstantBuffer>(InGraphics, *this, TransformConstantBuffer::Target::Vertex));
}

DirectX::XMMATRIX Ball::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll) *
		   DirectX::XMMatrixTranslation(X, 0.0f, 0.0f) *
		   DirectX::XMMatrixRotationRollPitchYaw(Theta, Phi, Chi);
}
