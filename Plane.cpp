#include "Plane.h"
#include "PlaneGeometry.h"
#include "Bindables.h"

Plane::Plane(const Graphics& InGraphics, const float InSize)
{
	auto Model = PlaneGeometry::Make();
	Model.Transform(DirectX::XMMatrixScaling(InSize, InSize, 1.0f));

	const auto GeometryTag = "$plane." + std::to_string( InSize );

	Bind(VertexBuffer::Resolve(InGraphics, GeometryTag, Model.Vertices));

	Bind(IndexBuffer::Resolve(InGraphics, GeometryTag, Model.Indices));

	Bind(Texture::Resolve(InGraphics, "Images\\brickwall.jpg"));
	Bind(Texture::Resolve(InGraphics, "Images\\brickwall_normal.jpg"));

	Bind(std::make_shared<Sampler>(InGraphics));

	auto ModelVertexShader = VertexShader::Resolve(InGraphics, "DiffuseNormalPhongVS.cso");
	auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
	Bind(std::move(ModelVertexShader));

	Bind(PixelShader::Resolve(InGraphics, "DiffuseNormalPhongPS.cso"));

	struct PSMaterialConstant
	{
		float specularIntensity = 0.1f;
		float specularPower = 20.0f;
		BOOL normalMappingEnabled = TRUE;
		float padding[1];
	} ModelMaterialConstants;

	struct PSCameraConstants
	{
		alignas(16) DirectX::XMFLOAT3 Position;
	} ModelCameraConstants;

	Bind(PixelConstantBuffer<PSMaterialConstant>::Resolve(InGraphics, ModelMaterialConstants, 1u));
	Bind(PixelConstantBuffer<PSCameraConstants>::Resolve(InGraphics, ModelCameraConstants, 2u));

	Bind(InputLayout::Resolve(InGraphics, Model.Vertices.GetLayout(), ModelVertexShaderBlob));

	Bind(Topology::Resolve(InGraphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	Bind(std::make_shared<TransformConstantBuffer>(InGraphics, *this, TransformConstantBuffer::Target::Vertex));
}

void Plane::SetPosition(const DirectX::XMFLOAT3 InPosition) noexcept
{
	Position = InPosition;
}

void Plane::SetRotation(const float InRoll, const float InPitch, const float InYaw) noexcept
{
	Roll = InRoll;
	Pitch = InPitch;
	Yaw = InYaw;
}

DirectX::XMMATRIX Plane::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(Roll, Pitch, Yaw) *
		   DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z);
}