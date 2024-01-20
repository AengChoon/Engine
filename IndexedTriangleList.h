#pragma once
#include <vector>
#include <DirectXMath.h>

template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> InVertices, std::vector<unsigned short> InIndices)
		: Vertices(std::move(InVertices)), Indices(std::move(InIndices))
	{
		assert(Vertices.size() > 2);
		assert(!(Indices.size() % 3));
	}

	void Transform(const DirectX::FXMMATRIX& InMatrix)
	{
		for (auto& Vertex : Vertices)
		{
			const DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&Vertex.Position);
			DirectX::XMStoreFloat3(&Vertex.Position, DirectX::XMVector3Transform(Position, InMatrix));
		}
	}

	void SetNormalsIndependentFlat()
	{
		for (size_t i = 0; i < Indices.size(); i += 3)
		{
			auto& Vertex_0 = Vertices[Indices[i]];
			auto& Vertex_1 = Vertices[Indices[i + 1]];
			auto& Vertex_2 = Vertices[Indices[i + 2]];
			
			const DirectX::XMVECTOR Position_0 = DirectX::XMLoadFloat3(&Vertex_0.Position);
			const DirectX::XMVECTOR Position_1 = DirectX::XMLoadFloat3(&Vertex_1.Position);
			const DirectX::XMVECTOR Position_2 = DirectX::XMLoadFloat3(&Vertex_2.Position);

			const auto Normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(Position_1, Position_0), DirectX::XMVectorSubtract(Position_2, Position_0)));

			DirectX::XMStoreFloat3(&Vertex_0.Normal, Normal);
			DirectX::XMStoreFloat3(&Vertex_1.Normal, Normal);
			DirectX::XMStoreFloat3(&Vertex_2.Normal, Normal);
		}
	}

public:
	std::vector<T> Vertices;
	std::vector<unsigned short> Indices;
};