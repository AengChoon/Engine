#pragma once
#include <array>
#include "DynamicVertex.h"
#include "EngineMath.h"
#include "IndexedTriangleList.h"

class PlaneGeometry
{
public:
	static IndexedTriangleList MakeTesselatedTextured(DV::VertexLayout InLayout,
													  const int InXDivisions,
													  const int InYDivisions)
	{
		assert(InXDivisions >= 1);
		assert(InYDivisions >= 1);

		constexpr float Width = 2.0f;
		constexpr float Height = 2.0f;
		const int XVerticesNum = InXDivisions + 1;
		const int YVerticesNum = InYDivisions + 1;
		DV::VertexBuffer PlaneVertexBuffer {std::move(InLayout)};

		const float XSize = Width / static_cast<float>(InXDivisions);
		const float YSize = Height / static_cast<float>(InYDivisions);
		const float XTextureCoordinateSize = 1.0f / static_cast<float>(InXDivisions);
		const float YTextureCoordinateSize = 1.0f / static_cast<float>(InYDivisions);

		for (int YIndex = 0; YIndex < YVerticesNum; YIndex++)
		{
			const float YPos = static_cast<float>(YIndex) * YSize - 1.0f;
			const float YTextureCoordinatePosition = 1.0f - static_cast<float>(YIndex) * YTextureCoordinateSize;

			for (int XIndex = 0; XIndex < XVerticesNum; XIndex++)
			{
				const float XPos = static_cast<float>(XIndex) * XSize - 1.0f;
				const float XTextureCoordinatePosition = static_cast<float>(XIndex) * XTextureCoordinateSize;

				PlaneVertexBuffer.Emplace
				(
					DirectX::XMFLOAT3{XPos, YPos, 0.0f},
					DirectX::XMFLOAT3{0.0f, 0.0f, -1.0f},
					DirectX::XMFLOAT2{XTextureCoordinatePosition, YTextureCoordinatePosition}
				);
			}
		}

		std::vector<unsigned int> PlaneIndices;
		PlaneIndices.reserve(Math::Square(InXDivisions * InYDivisions) * 6);

		const auto VertexToIndex = [XVerticesNum](const size_t InX, const size_t InY)
		{
			return static_cast<unsigned int>(InY * XVerticesNum + InX);
		};

		for (size_t YIndex = 0; YIndex < InYDivisions; YIndex++)
		{
			for (size_t XIndex = 0; XIndex < InXDivisions; XIndex++)
			{
				const std::array IndexArray =
				{
					VertexToIndex(XIndex, YIndex),
					VertexToIndex(XIndex + 1, YIndex),
					VertexToIndex(XIndex, YIndex + 1),
					VertexToIndex(XIndex + 1, YIndex + 1)
				};

				PlaneIndices.push_back(IndexArray[0]);
				PlaneIndices.push_back(IndexArray[2]);
				PlaneIndices.push_back(IndexArray[1]);
				PlaneIndices.push_back(IndexArray[1]);
				PlaneIndices.push_back(IndexArray[2]);
				PlaneIndices.push_back(IndexArray[3]);
			}
		}

		return {std::move(PlaneVertexBuffer), std::move(PlaneIndices)};
	}

	static IndexedTriangleList Make()
	{
		DV::VertexLayout PlaneVertexLayout
		{
			DV::VertexLayout::ElementType::Position3D,
			DV::VertexLayout::ElementType::Normal,
			DV::VertexLayout::ElementType::Texture2D
		};

		return MakeTesselatedTextured(std::move(PlaneVertexLayout), 1, 1);
	}
};