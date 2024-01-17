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

public:
	std::vector<T> Vertices;
	std::vector<unsigned short> Indices;
};