#pragma once
#include <vector>
#include <DirectXMath.h>

class IndexedTriangleList
{
public:
	IndexedTriangleList() = delete;
	IndexedTriangleList(DV::VertexBuffer InVertices, std::vector<unsigned int> InIndices)
		: Vertices(std::move(InVertices)), Indices(std::move(InIndices))
	{
		assert(Vertices.Num() > 2);
		assert(!(Indices.size() % 3));
	}

	void Transform(const DirectX::FXMMATRIX& InMatrix)
	{
		using Element = DV::VertexLayout::ElementType;

		for (int VertexIndex = 0; VertexIndex < Vertices.Num(); ++VertexIndex)
		{
			auto& Position = Vertices[VertexIndex].Attribute<Element::Position3D>();
			DirectX::XMStoreFloat3(&Position, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&Position), InMatrix));
		}
	}

	void SetNormalsIndependentFlat()
	{
		using Element = DV::VertexLayout::ElementType;

		for (size_t i = 0; i < Indices.size(); i += 3)
		{
			DV::Vertex Vertex_0 {Vertices[Indices[i]]};
			DV::Vertex Vertex_1 {Vertices[Indices[i + 1]]};
			DV::Vertex Vertex_2 {Vertices[Indices[i + 2]]};
			
			const DirectX::XMVECTOR Position_0 = DirectX::XMLoadFloat3(&Vertex_0.Attribute<Element::Position3D>());
			const DirectX::XMVECTOR Position_1 = DirectX::XMLoadFloat3(&Vertex_1.Attribute<Element::Position3D>());
			const DirectX::XMVECTOR Position_2 = DirectX::XMLoadFloat3(&Vertex_2.Attribute<Element::Position3D>());

			const auto Normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(Position_1, Position_0), DirectX::XMVectorSubtract(Position_2, Position_0)));

			DirectX::XMStoreFloat3(&Vertex_0.Attribute<Element::Normal>(), Normal);
			DirectX::XMStoreFloat3(&Vertex_1.Attribute<Element::Normal>(), Normal);
			DirectX::XMStoreFloat3(&Vertex_2.Attribute<Element::Normal>(), Normal);
		}
	}

public:
	DV::VertexBuffer Vertices;
	std::vector<unsigned int> Indices;
};