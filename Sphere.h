#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "EngineMath.h"

class Sphere
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTessellated<V>(12, 24);
	}

	template<class V>
	static IndexedTriangleList<V> MakeTessellated(const int InLatitudeDiv, const int InLongitudeDiv)
	{
		assert(InLatitudeDiv >= 3);
		assert(InLongitudeDiv >= 3);

		constexpr float Radius = 1.0f;
		const auto Base = DirectX::XMVectorSet(0.0f, 0.0f, Radius, 0.0f);
		const float LatitudeAngle = Math::PI / InLatitudeDiv;
		const float LongitudeAngle = 2.0f * Math::PI / InLongitudeDiv;

		std::vector<V> Vertices;
		for (int LatitudeIndex = 1; LatitudeIndex < InLatitudeDiv; LatitudeIndex++)
		{
			const auto latBase = DirectX::XMVector3Transform(Base, DirectX::XMMatrixRotationX(LatitudeAngle * LatitudeIndex));

			for (int LongitudeIndex = 0; LongitudeIndex < InLongitudeDiv; ++LongitudeIndex)
			{
				Vertices.emplace_back();
				auto Vertex = DirectX::XMVector3Transform(latBase, DirectX::XMMatrixRotationZ(LongitudeAngle * LongitudeIndex));
				DirectX::XMStoreFloat3(&Vertices.back().Position, Vertex);
			}
		}

		const auto NorthPoleVertex = static_cast<unsigned short>(Vertices.size());
		Vertices.emplace_back();
		DirectX::XMStoreFloat3(&Vertices.back().Position, Base);

		const auto SouthPoleVertex = static_cast<unsigned short>(Vertices.size());
		Vertices.emplace_back();
		DirectX::XMStoreFloat3(&Vertices.back().Position, DirectX::XMVectorNegate(Base));

		const auto CalculateIndex = [InLongitudeDiv](const unsigned short InLatitudeIndex, const unsigned short InLongitudeIndex)
		{
			return InLatitudeIndex * InLongitudeDiv + InLongitudeIndex;
		};

		std::vector<unsigned int> Indices;
		for (unsigned short LatitudeIndex = 0; LatitudeIndex < InLatitudeDiv - 2; ++LatitudeIndex)
		{
			for (unsigned short LongitudeIndex = 0; LongitudeIndex < InLongitudeDiv - 1; ++LongitudeIndex)
			{
				Indices.push_back(CalculateIndex(LatitudeIndex, LongitudeIndex));
				Indices.push_back(CalculateIndex(LatitudeIndex + 1, LongitudeIndex));
				Indices.push_back(CalculateIndex(LatitudeIndex, LongitudeIndex + 1));
				Indices.push_back(CalculateIndex(LatitudeIndex, LongitudeIndex + 1));
				Indices.push_back(CalculateIndex(LatitudeIndex + 1, LongitudeIndex));
				Indices.push_back(CalculateIndex(LatitudeIndex + 1, LongitudeIndex + 1));
			}

			Indices.push_back(CalculateIndex(LatitudeIndex, InLongitudeDiv - 1));
			Indices.push_back(CalculateIndex(LatitudeIndex + 1, InLongitudeDiv - 1));
			Indices.push_back(CalculateIndex(LatitudeIndex, 0));
			Indices.push_back(CalculateIndex(LatitudeIndex, 0));
			Indices.push_back(CalculateIndex(LatitudeIndex + 1, InLongitudeDiv - 1));
			Indices.push_back(CalculateIndex(LatitudeIndex + 1, 0));			
		}

		for (unsigned short LongitudeIndex = 0; LongitudeIndex < InLongitudeDiv - 1; ++LongitudeIndex)
		{
			Indices.push_back(NorthPoleVertex);
			Indices.push_back(CalculateIndex(0, LongitudeIndex));
			Indices.push_back(CalculateIndex(0, LongitudeIndex + 1));

			Indices.push_back(CalculateIndex(InLatitudeDiv - 2,LongitudeIndex + 1));
			Indices.push_back(CalculateIndex(InLatitudeDiv - 2,LongitudeIndex));
			Indices.push_back(SouthPoleVertex);
		}

		Indices.push_back(NorthPoleVertex);
		Indices.push_back(CalculateIndex(0, InLongitudeDiv - 1));
		Indices.push_back(CalculateIndex(0, 0));

		Indices.push_back(CalculateIndex(InLatitudeDiv - 2, 0));
		Indices.push_back(CalculateIndex(InLatitudeDiv - 2, InLongitudeDiv - 1));
		Indices.push_back(SouthPoleVertex);

		return {std::move(Vertices), std::move(Indices)};
	}
};