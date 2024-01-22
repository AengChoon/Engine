#include "Box.h"
#include "Bindables.h"
#include "Camera.h"
#include "Cube.h"

Box::Box(Graphics& InGraphics, std::mt19937& InRandomGenerator, 
         std::uniform_real_distribution<float>& InA, std::uniform_real_distribution<float>& InB,
         std::uniform_real_distribution<float>& InC, std::uniform_real_distribution<float>& InD,
		 std::uniform_real_distribution<float>& InE, DirectX::XMFLOAT3 InMaterialColor)
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
			DirectX::XMFLOAT3 Normal;
		};

		auto Model = Cube::MakeIndependent<Vertex>();
		Model.SetNormalsIndependentFlat();

		AddStaticBindable(std::make_unique<VertexBuffer>(InGraphics, Model.Vertices));

		auto BoxVertexShader = std::make_unique<VertexShader>(InGraphics, L"PhongVS.cso");
		auto BoxVertexShaderBlob = BoxVertexShader->GetByteCode();
		AddStaticBindable(std::move(BoxVertexShader));

		AddStaticBindable(std::make_unique<PixelShader>(InGraphics, L"PhongPS.cso"));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(InGraphics, Model.Indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		AddStaticBindable(std::make_unique<InputLayout>(InGraphics, InputElementDescs, BoxVertexShaderBlob));
		AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexBufferFromStaticBindables();
	}

	struct PSMaterialConstants
	{
		alignas(16) DirectX::XMFLOAT3 Color;
		float SpecularIntensity = 0.6f;
		float SpecularPower = 30.0f;
		float Padding[2];
	} MaterialConstants;

	MaterialConstants.Color = InMaterialColor;
	AddInstanceBindable(std::make_unique<PixelConstantBuffer<PSMaterialConstants>>(InGraphics, MaterialConstants, 1u));

	struct PSCameraConstants
	{
		alignas(16) DirectX::XMFLOAT3 Position;
	} CameraConstants;

	CameraConstants.Position = InGraphics.GetCamera().GetPosition();
	AddInstanceBindable(std::make_unique<PixelConstantBuffer<PSCameraConstants>>(InGraphics, CameraConstants, 2u));

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
		   DirectX::XMMatrixRotationRollPitchYaw(Theta, Phi, Chi);
}
