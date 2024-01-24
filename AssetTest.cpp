#include "AssetTest.h"
#include "Bindables.h"
#include "Camera.h"
#include "DynamicVertex.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

AssetTest::AssetTest(const Graphics& InGraphics, std::mt19937& InRandomEngine,
                     std::uniform_real_distribution<float>& InPositionDistribution,
                     std::uniform_real_distribution<float>& InRotationDistribution,
                     const DirectX::XMFLOAT3 InMaterialColor, const float InScale)
	: TestObject(InGraphics, InRandomEngine, InPositionDistribution, InRotationDistribution)
{
	if (!IsStaticInitialized())
	{
		DV::VertexBuffer ModelVertexBuffer
		{
			DV::VertexLayout{DV::VertexLayout::ElementType::Position3D, DV::VertexLayout::ElementType::Normal}
		};

		Assimp::Importer Importer;
		const auto Model = Importer.ReadFile("Models\\suzanne.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		const auto Mesh = Model->mMeshes[0];
		
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
		{
			ModelVertexBuffer.Emplace
			(
				DirectX::XMFLOAT3
				{
					Mesh->mVertices[i].x * InScale,
					Mesh->mVertices[i].y * InScale,
					Mesh->mVertices[i].z * InScale
				},
				*reinterpret_cast<DirectX::XMFLOAT3*>(&Mesh->mNormals[i])
			);
		}

		std::vector<unsigned short> Indices;
		Indices.reserve(Mesh->mNumFaces * 3);
		for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
		{
			const auto& Face = Mesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			Indices.push_back(Face.mIndices[0]);
			Indices.push_back(Face.mIndices[1]);
			Indices.push_back(Face.mIndices[2]);
		}

		AddStaticBindable(std::make_unique<VertexBuffer>(InGraphics, ModelVertexBuffer));

		AddStaticIndexBuffer( std::make_unique<IndexBuffer>(InGraphics, Indices));

		auto ModelVertexShader = std::make_unique<VertexShader>(InGraphics, L"PhongVS.cso");
		auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
		AddStaticBindable(std::move(ModelVertexShader));

		AddStaticBindable(std::make_unique<PixelShader>(InGraphics, L"PhongPS.cso"));

		AddStaticBindable(std::make_unique<InputLayout>(InGraphics, ModelVertexBuffer.GetLayout().GetD3D11Layout(), ModelVertexShaderBlob));
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
		float SpecularPower = 10.0f;
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
}
