#include "Sphere.h"
#include "EngineMath.h"

IndexedTriangleList Sphere::Make(DV::VertexLayout InLayout)
{
	if (!InLayout.Size())
	{
		InLayout.Append(DV::VertexLayout::ElementType::Position3D);
	}

	return MakeTessellated(std::move(InLayout), 12, 24);
}

IndexedTriangleList Sphere::MakeTessellated(DV::VertexLayout&& InLayout, const int InLatitudeDiv, const int InLongitudeDiv)
{
	assert(InLatitudeDiv >= 3);
	assert(InLongitudeDiv >= 3);

	constexpr float Radius = 1.0f;
	const auto Base = DirectX::XMVectorSet(0.0f, 0.0f, Radius, 0.0f);
	const float LatitudeAngle = Math::PI / InLatitudeDiv;
	const float LongitudeAngle = 2.0f * Math::PI / InLongitudeDiv;

	DV::VertexBuffer SphereVertexBuffer {std::move(InLayout)};
	for (int LatitudeIndex = 1; LatitudeIndex < InLatitudeDiv; LatitudeIndex++)
	{
		const auto latBase = DirectX::XMVector3Transform(Base, DirectX::XMMatrixRotationX(LatitudeAngle * LatitudeIndex));

		for (int LongitudeIndex = 0; LongitudeIndex < InLongitudeDiv; ++LongitudeIndex)
		{
			DirectX::XMFLOAT3 VertexPosition;
			DirectX::XMStoreFloat3(&VertexPosition, DirectX::XMVector3Transform(latBase, DirectX::XMMatrixRotationZ(LongitudeAngle * LongitudeIndex)));
			SphereVertexBuffer.Emplace(VertexPosition);
		}
	}

	const auto NorthPoleVertex = static_cast<unsigned short>(SphereVertexBuffer.Size());
	DirectX::XMFLOAT3 NorthPoleVertexPosition;
	DirectX::XMStoreFloat3(&NorthPoleVertexPosition, Base);
	SphereVertexBuffer.Emplace(NorthPoleVertexPosition);

	const auto SouthPoleVertex = static_cast<unsigned short>(SphereVertexBuffer.Size());
	DirectX::XMFLOAT3 SouthPoleVertexPosition;
	DirectX::XMStoreFloat3(&SouthPoleVertexPosition, DirectX::XMVectorNegate(Base));
	SphereVertexBuffer.Emplace(SouthPoleVertexPosition);

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

	return {std::move(SphereVertexBuffer), std::move(Indices)};
}