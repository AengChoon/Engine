#include "Mesh.h"
#include <unordered_map>
#include "Bindables.h"
#include "Camera.h"
#include "PointLight.h"
#include "Surface.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "imgui/imgui.h"

Mesh::Mesh(const Graphics& InGraphics, std::vector<std::shared_ptr<Bindable>>&& InBindables)
{
	Bind(Topology::Resolve(InGraphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	for (auto& Bindable : InBindables)
	{
		Bind(std::move(Bindable));
	}

	Bind(std::make_unique<TransformConstantBuffer>(InGraphics, *this, TransformConstantBuffer::Target::Vertex));
}

void Mesh::Draw(const Graphics& InGraphics, const DirectX::XMMATRIX& InAccumulatedTransform) const
{
	DirectX::XMStoreFloat4x4(&TransformMatrix, InAccumulatedTransform);
	Drawable::Draw(InGraphics);
}

DirectX::XMMATRIX Mesh::GetTransformMatrix() const noexcept
{
	return DirectX::XMLoadFloat4x4(&TransformMatrix);
}

Node::Node(int InID, std::string_view InName, std::vector<Mesh*>&& InMeshes, const DirectX::XMMATRIX& InTransformMatrix)
	: Name(InName), ID(InID), Meshes(std::move(InMeshes))
{
	DirectX::XMStoreFloat4x4(&BaseTransform, InTransformMatrix);
	DirectX::XMStoreFloat4x4(&AppliedTransform, DirectX::XMMatrixIdentity());
}

void Node::Draw(const Graphics& InGraphics, const DirectX::XMMATRIX& InAccumulatedTransformMatrix) const
{
	const auto MyTransformMatrix = DirectX::XMLoadFloat4x4(&AppliedTransform) *
										   DirectX::XMLoadFloat4x4(&BaseTransform)*
										   InAccumulatedTransformMatrix;

	for (const auto* Mesh : Meshes)
	{
		Mesh->Draw(InGraphics, MyTransformMatrix);
	}

	for (const auto& Child : Children)
	{
		Child->Draw(InGraphics, MyTransformMatrix);
	}
}

void Node::SetAppliedTransform(DirectX::FXMMATRIX InTransform)
{
	DirectX::XMStoreFloat4x4(&AppliedTransform, InTransform);
}

int Node::GetID() const noexcept
{
	return ID;
}

void Node::AddChild(std::unique_ptr<Node>&& InNode)
{
	Children.push_back(std::move(InNode));
}

void Node::ShowTree(const Node** InSelectedNode) const
{
	const int IsCurrentNodeSelected = GetID() == (*InSelectedNode)->GetID();
	const int IsCurrentNodeLeaf = Children.empty();

	const auto NodeFlags = ImGuiTreeNodeFlags_OpenOnArrow |
							   (IsCurrentNodeSelected ? ImGuiTreeNodeFlags_Selected : 0) |
							   (IsCurrentNodeLeaf ? ImGuiTreeNodeFlags_Leaf : 0);

	const auto IsExpanded = ImGui::TreeNodeEx
								 (
									reinterpret_cast<void*>(static_cast<intptr_t>(GetID())),
									NodeFlags,
									Name.c_str()
								 );

	if (ImGui::IsItemClicked())
	{
		*InSelectedNode = this;
	}

	if (IsExpanded)
	{
		for (const auto& Child : Children)
		{
			Child->ShowTree(InSelectedNode);
		}

		ImGui::TreePop();
	}
}

class ModelWindow
{
public:
	ModelWindow(const int InDefaultSelectedNodeIndex, Node* InDefaultSelectedNode)
		: SelectedNodeIndex(InDefaultSelectedNodeIndex), SelectedNode(InDefaultSelectedNode)
	{
		NodeTransforms[SelectedNode->GetID()] = {};
	}

	void Show(const std::string_view InWindowName, const Node& InRoot) noexcept
	{
		const auto WindowName = !InWindowName.empty() ? InWindowName : "Model";

		if (ImGui::Begin(WindowName.data()))
		{
			ImGui::Columns(2, nullptr, true);
			InRoot.ShowTree(const_cast<const Node**>(&SelectedNode));

			ImGui::NextColumn();

			if (SelectedNode)
			{
				auto& [Roll, Pitch, Yaw, X, Y, Z] = NodeTransforms[SelectedNode->GetID()];
				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &Roll, -180.0f, 180.0f);
				ImGui::SliderAngle("Pitch", &Pitch, -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw", &Yaw, -180.0f, 180.0f);
				ImGui::Text("Position");
				ImGui::SliderFloat("X", &X, -20.0f, 20.0f);
				ImGui::SliderFloat("Y", &Y, -20.0f, 20.0f);
				ImGui::SliderFloat("Z", &Z, -20.0f, 20.0f);
			}
		}

		ImGui::End();
	}

	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept
	{
		const auto& [Roll, Pitch, Yaw, X, Y, Z] = NodeTransforms.at(SelectedNode->GetID());
		return DirectX::XMMatrixRotationRollPitchYaw(Roll, Pitch, Yaw) *
			   DirectX::XMMatrixTranslation(X, Y, Z);
	}

	[[nodiscard]] Node* GetSelectedNode() const
	{
		return SelectedNode;
	}

private:
	struct TransformParameters
	{
		float Roll = 0.0f;
		float Pitch = 0.0f;
		float Yaw = 0.0f;
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;
	};

	int SelectedNodeIndex;
	Node* SelectedNode;
	std::unordered_map<int, TransformParameters> NodeTransforms;
};

Model::Model(const Graphics& InGraphics, const std::string_view InFileName)
{
	Assimp::Importer Importer;
	const auto* Scene = Importer.ReadFile
	(
		InFileName.data(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	for (size_t MeshIndex = 0; MeshIndex < Scene->mNumMeshes; ++MeshIndex)
	{
		Meshes.push_back(ParseMesh(InGraphics, *Scene->mMeshes[MeshIndex], Scene->mMaterials));
	}

	int NextID = 0;
	Root = ParseNode(NextID, *Scene->mRootNode);
	Window = std::make_unique<ModelWindow>(0, Root.get());
}

Model::~Model() = default;

std::unique_ptr<Mesh> Model::ParseMesh(const Graphics& InGraphics, const aiMesh& InMesh, aiMaterial const* const* InMaterials)
{
	const std::string& BaseDirectory {R"(Models\nanosuit_textured\)"};

	std::vector<std::shared_ptr<Bindable>> Bindables;
	bool bHasDiffuseMap {false};
	bool bHasNormalMap {false};
	bool bHasSpecularMap {false};
	float Shininess {35.0f};

	auto& Material = *InMaterials[InMesh.mMaterialIndex];
	aiString TextureFileName;

	if (Material.GetTexture(aiTextureType_DIFFUSE, 0, &TextureFileName) == aiReturn_SUCCESS)
	{
		Bindables.push_back(Texture::Resolve(InGraphics, BaseDirectory + TextureFileName.C_Str()));
		bHasDiffuseMap = true;
	}

	if (Material.GetTexture(aiTextureType_NORMALS, 0, &TextureFileName) == aiReturn_SUCCESS)
	{
		Bindables.push_back(Texture::Resolve(InGraphics, BaseDirectory + TextureFileName.C_Str(), 1));
		bHasNormalMap = true;
	}

	if (Material.GetTexture(aiTextureType_SPECULAR, 0, &TextureFileName) == aiReturn_SUCCESS)
	{
		Bindables.push_back(Texture::Resolve(InGraphics, BaseDirectory + TextureFileName.C_Str(), 2));
		bHasSpecularMap = true;
	}
	else
	{
		Material.Get(AI_MATKEY_SHININESS, Shininess);
	}

	if (bHasDiffuseMap || bHasNormalMap || bHasSpecularMap)
	{
		Bindables.push_back(Sampler::Resolve(InGraphics));
	}

	const auto MeshTag {BaseDirectory + "$" + InMesh.mName.C_Str()};
	constexpr float Scale {1.0f};

	if (bHasDiffuseMap && bHasNormalMap && bHasSpecularMap)
	{
		DV::VertexBuffer ModelVertexBuffer
		{
			{
				DV::VertexLayout::ElementType::Position3D,
				DV::VertexLayout::ElementType::Normal,
				DV::VertexLayout::ElementType::Tangent,
				DV::VertexLayout::ElementType::Bitangent,
				DV::VertexLayout::ElementType::Texture2D
			}
		};

		for (unsigned int VertexIndex = 0; VertexIndex < InMesh.mNumVertices; ++VertexIndex)
		{
			ModelVertexBuffer.Emplace
			(
				DirectX::XMFLOAT3
				{
					InMesh.mVertices[VertexIndex].x * Scale,
					InMesh.mVertices[VertexIndex].y * Scale,
					InMesh.mVertices[VertexIndex].z * Scale,
				},
				*reinterpret_cast<DirectX::XMFLOAT3*>(&InMesh.mNormals[VertexIndex]),
				*reinterpret_cast<DirectX::XMFLOAT3*>(&InMesh.mTangents[VertexIndex]),
				*reinterpret_cast<DirectX::XMFLOAT3*>(&InMesh.mBitangents[VertexIndex]),
				*reinterpret_cast<DirectX::XMFLOAT2*>(&InMesh.mTextureCoords[0][VertexIndex])
			);
		}

		std::vector<unsigned int> Indices;
		Indices.reserve(InMesh.mNumFaces * 3);
		for (unsigned int FaceIndex = 0; FaceIndex < InMesh.mNumFaces; ++FaceIndex)
		{
			const auto& Face = InMesh.mFaces[FaceIndex];
			assert(Face.mNumIndices == 3);

			for (unsigned int Index = 0; Index < Face.mNumIndices; ++Index)
			{
				Indices.push_back(Face.mIndices[Index]);
			}
		}

		Bindables.push_back(VertexBuffer::Resolve(InGraphics, MeshTag, ModelVertexBuffer));
		Bindables.push_back(IndexBuffer::Resolve(InGraphics, MeshTag, Indices));

		auto ModelVertexShader = VertexShader::Resolve(InGraphics, "DiffuseNormalPhongVS.cso");
		auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
		Bindables.push_back(std::move(ModelVertexShader));

		Bindables.push_back(PixelShader::Resolve(InGraphics, "DiffuseNormalSpecularPhongPS.cso"));

		struct PSDiffuseNormalSpecularConstants
		{
			BOOL bIsNormalMapEnabled {TRUE};
			float Padding[3];
		} DiffuseNormalSpecularConstants;

		Bindables.push_back(PixelConstantBuffer<PSDiffuseNormalSpecularConstants>::Resolve(InGraphics, DiffuseNormalSpecularConstants, 1u));

		struct PSCameraConstants
		{
			alignas(16) DirectX::XMFLOAT3 Position;
		} CameraConstants;
		CameraConstants.Position = InGraphics.GetCamera().GetPosition();

		Bindables.push_back(PixelConstantBuffer<PSCameraConstants>::Resolve(InGraphics, CameraConstants, 2u));

		Bindables.push_back(InputLayout::Resolve(InGraphics, ModelVertexBuffer.GetLayout(), ModelVertexShaderBlob));
	}
	else if (bHasDiffuseMap && bHasNormalMap)
	{
		DV::VertexBuffer ModelVertexBuffer
		{
			{
				DV::VertexLayout::ElementType::Position3D,
				DV::VertexLayout::ElementType::Normal,
				DV::VertexLayout::ElementType::Tangent,
				DV::VertexLayout::ElementType::Bitangent,
				DV::VertexLayout::ElementType::Texture2D
			}
		};

		for (unsigned int VertexIndex = 0; VertexIndex < InMesh.mNumVertices; ++VertexIndex)
		{
			ModelVertexBuffer.Emplace
			(
				DirectX::XMFLOAT3
				{
					InMesh.mVertices[VertexIndex].x * Scale,
					InMesh.mVertices[VertexIndex].y * Scale,
					InMesh.mVertices[VertexIndex].z * Scale,
				},
				*reinterpret_cast<DirectX::XMFLOAT3*>(&InMesh.mNormals[VertexIndex]),
				*reinterpret_cast<DirectX::XMFLOAT3*>(&InMesh.mTangents[VertexIndex]),
				*reinterpret_cast<DirectX::XMFLOAT3*>(&InMesh.mBitangents[VertexIndex]),
				*reinterpret_cast<DirectX::XMFLOAT2*>(&InMesh.mTextureCoords[0][VertexIndex])
			);
		}

		std::vector<unsigned int> Indices;
		Indices.reserve(InMesh.mNumFaces * 3);
		for (unsigned int FaceIndex = 0; FaceIndex < InMesh.mNumFaces; ++FaceIndex)
		{
			const auto& Face = InMesh.mFaces[FaceIndex];
			assert(Face.mNumIndices == 3);

			for (unsigned int Index = 0; Index < Face.mNumIndices; ++Index)
			{
				Indices.push_back(Face.mIndices[Index]);
			}
		}

		Bindables.push_back(VertexBuffer::Resolve(InGraphics, MeshTag, ModelVertexBuffer));

		Bindables.push_back(IndexBuffer::Resolve(InGraphics, MeshTag, Indices));

		auto ModelVertexShader = VertexShader::Resolve(InGraphics, "DiffuseNormalPhongVS.cso");
		auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
		Bindables.push_back(std::move(ModelVertexShader));

		Bindables.push_back(PixelShader::Resolve(InGraphics, "DiffuseNormalPhongPS.cso"));

		struct PSDiffuseNormalConstants
		{
			float SpecularIntensity {0.18f};
			float SpecularPower;
			BOOL bIsNormalMapEnabled {TRUE};
			float Padding[1];
		} DiffuseNormalConstants;
		DiffuseNormalConstants.SpecularPower = Shininess;

		Bindables.push_back(PixelConstantBuffer<PSDiffuseNormalConstants>::Resolve(InGraphics, DiffuseNormalConstants, 1u));

		struct PSCameraConstants
		{
			alignas(16) DirectX::XMFLOAT3 Position;
		} CameraConstants;

		CameraConstants.Position = InGraphics.GetCamera().GetPosition();
		Bindables.push_back(PixelConstantBuffer<PSCameraConstants>::Resolve(InGraphics, CameraConstants, 2u));

		Bindables.push_back(InputLayout::Resolve(InGraphics, ModelVertexBuffer.GetLayout(), ModelVertexShaderBlob));
	}
	else if (bHasDiffuseMap)
	{
		DV::VertexBuffer ModelVertexBuffer
		{
			{
				DV::VertexLayout::ElementType::Position3D,
				DV::VertexLayout::ElementType::Normal,
				DV::VertexLayout::ElementType::Texture2D
			}
		};

		for (unsigned int VertexIndex = 0; VertexIndex < InMesh.mNumVertices; ++VertexIndex)
		{
			ModelVertexBuffer.Emplace
			(
				DirectX::XMFLOAT3
				{
					InMesh.mVertices[VertexIndex].x * Scale,
					InMesh.mVertices[VertexIndex].y * Scale,
					InMesh.mVertices[VertexIndex].z * Scale,
				},
				*reinterpret_cast<DirectX::XMFLOAT3*>(&InMesh.mNormals[VertexIndex]),
				*reinterpret_cast<DirectX::XMFLOAT2*>(&InMesh.mTextureCoords[0][VertexIndex])
			);
		}

		std::vector<unsigned int> Indices;
		Indices.reserve(InMesh.mNumFaces * 3);
		for (unsigned int FaceIndex = 0; FaceIndex < InMesh.mNumFaces; ++FaceIndex)
		{
			const auto& Face = InMesh.mFaces[FaceIndex];
			assert(Face.mNumIndices == 3);

			for (unsigned int Index = 0; Index < Face.mNumIndices; ++Index)
			{
				Indices.push_back(Face.mIndices[Index]);
			}
		}

		Bindables.push_back(VertexBuffer::Resolve(InGraphics, MeshTag, ModelVertexBuffer));
		Bindables.push_back(IndexBuffer::Resolve(InGraphics, MeshTag, Indices));

		auto ModelVertexShader = VertexShader::Resolve(InGraphics, "DiffusePhongVS.cso");
		auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
		Bindables.push_back(std::move(ModelVertexShader));

		Bindables.push_back(PixelShader::Resolve(InGraphics, "DiffusePhongPS.cso"));

		struct PSDiffuseMaterialConstants
		{
			float SpecularIntensity {0.01f};
			float SpecularPower;
			float Padding[2];
		} DiffuseMaterialConstants;
		DiffuseMaterialConstants.SpecularPower = Shininess;

		Bindables.push_back(PixelConstantBuffer<PSDiffuseMaterialConstants>::Resolve(InGraphics, DiffuseMaterialConstants, 1u));

		struct PSCameraConstants
		{
			alignas(16) DirectX::XMFLOAT3 Position;
		} CameraConstants;

		CameraConstants.Position = InGraphics.GetCamera().GetPosition();
		Bindables.push_back(PixelConstantBuffer<PSCameraConstants>::Resolve(InGraphics, CameraConstants, 2u));

		Bindables.push_back(InputLayout::Resolve(InGraphics, ModelVertexBuffer.GetLayout(), ModelVertexShaderBlob));
	}
	else if (!bHasDiffuseMap && !bHasNormalMap && !bHasSpecularMap)
	{
		DV::VertexBuffer ModelVertexBuffer
		{
			{
				DV::VertexLayout::ElementType::Position3D,
				DV::VertexLayout::ElementType::Normal,
			}
		};

		for (unsigned int VertexIndex = 0; VertexIndex < InMesh.mNumVertices; ++VertexIndex)
		{
			ModelVertexBuffer.Emplace
			(
				DirectX::XMFLOAT3
				{
					InMesh.mVertices[VertexIndex].x * Scale,
					InMesh.mVertices[VertexIndex].y * Scale,
					InMesh.mVertices[VertexIndex].z * Scale,
				},
				*reinterpret_cast<DirectX::XMFLOAT3*>(&InMesh.mNormals[VertexIndex])
			);
		}

		std::vector<unsigned int> Indices;
		Indices.reserve(InMesh.mNumFaces * 3);
		for (unsigned int FaceIndex = 0; FaceIndex < InMesh.mNumFaces; ++FaceIndex)
		{
			const auto& Face = InMesh.mFaces[FaceIndex];
			assert(Face.mNumIndices == 3);

			for (unsigned int Index = 0; Index < Face.mNumIndices; ++Index)
			{
				Indices.push_back(Face.mIndices[Index]);
			}
		}

		Bindables.push_back(VertexBuffer::Resolve(InGraphics, MeshTag, ModelVertexBuffer));
		Bindables.push_back(IndexBuffer::Resolve(InGraphics, MeshTag, Indices));

		auto ModelVertexShader = VertexShader::Resolve(InGraphics, "PhongVS.cso");
		auto ModelVertexShaderBlob = ModelVertexShader->GetByteCode();
		Bindables.push_back(std::move(ModelVertexShader));

		Bindables.push_back(PixelShader::Resolve(InGraphics, "PhongPS.cso"));

		struct PSConstants
		{
			DirectX::XMFLOAT4 MaterialColor {0.65f, 0.65f, 0.85f, 1.0f};
			float SpecularIntensity {0.18f};
			float SpecularPower;
			float Padding[2];
		} MaterialConstants;
		MaterialConstants.SpecularPower = Shininess;

		Bindables.push_back(PixelConstantBuffer<PSConstants>::Resolve(InGraphics, MaterialConstants, 1u));

		struct PSCameraConstants
		{
			alignas(16) DirectX::XMFLOAT3 Position;
		} CameraConstants;

		CameraConstants.Position = InGraphics.GetCamera().GetPosition();
		Bindables.push_back(PixelConstantBuffer<PSCameraConstants>::Resolve(InGraphics, CameraConstants, 2u));

		Bindables.push_back(InputLayout::Resolve(InGraphics, ModelVertexBuffer.GetLayout(), ModelVertexShaderBlob));
	}

	return std::make_unique<Mesh>(InGraphics, std::move(Bindables));
}

std::unique_ptr<Node> Model::ParseNode(int& InNextID, const aiNode& InNode)
{
	const auto NodeTransformMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&InNode.mTransformation)));

	std::vector<Mesh*> NodeMeshes;
	NodeMeshes.reserve(InNode.mNumMeshes);

	for (size_t NodeMeshIndex = 0; NodeMeshIndex < InNode.mNumMeshes; ++NodeMeshIndex)
	{
		const auto ModelMeshIndex = InNode.mMeshes[NodeMeshIndex];
		NodeMeshes.push_back(Meshes.at(ModelMeshIndex).get());
	}

	auto NewNode = std::make_unique<Node>(InNextID++, InNode.mName.C_Str(), std::move(NodeMeshes), NodeTransformMatrix);

	for (size_t ChildrenIndex = 0; ChildrenIndex < InNode.mNumChildren; ++ChildrenIndex)
	{
		NewNode->AddChild(ParseNode(InNextID, *InNode.mChildren[ChildrenIndex]));
	}

	return NewNode;
}

void Model::Draw(const Graphics& InGraphics) const
{
	if (auto* const SelectedNode = Window->GetSelectedNode())
	{
		SelectedNode->SetAppliedTransform(Window->GetTransformMatrix());
	}

	Root->Draw(InGraphics, DirectX::XMMatrixIdentity());
}

void Model::ShowWindow(const std::string_view InWindowName) const
{
	Window->Show(InWindowName, *Root);
}
