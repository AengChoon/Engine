#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Cube
{
public:
	template<typename T>
	static IndexedTriangleList<T> Make()
	{
		constexpr float EdgeLength = 1.0f / 2.0f;

		std::vector<DirectX::XMFLOAT3> Positions;
		Positions.emplace_back(-EdgeLength,-EdgeLength, -EdgeLength);		// 0
		Positions.emplace_back(EdgeLength,-EdgeLength, -EdgeLength);		// 1
		Positions.emplace_back(-EdgeLength,EdgeLength, -EdgeLength);		// 2
		Positions.emplace_back(EdgeLength,EdgeLength, -EdgeLength);		// 3
		Positions.emplace_back(-EdgeLength,-EdgeLength, EdgeLength);		// 4
		Positions.emplace_back(EdgeLength,-EdgeLength, EdgeLength);		// 5
		Positions.emplace_back(-EdgeLength,EdgeLength, EdgeLength);		// 6
		Positions.emplace_back(EdgeLength, EdgeLength, EdgeLength);		// 7

		std::vector<T> Vertices {Positions.size()};
		for (size_t Index = 0; Index < Positions.size(); ++Index)
		{
			Vertices[Index].Position = Positions[Index];
		}

		return {
					std::move(Vertices),
					{
						0,2,1, 2,3,1,
						1,3,5, 3,7,5,
						2,6,3, 3,6,7,
						4,5,7, 4,7,6,
						0,4,2, 2,4,6,
						0,1,4, 1,5,4
					}
				};
	}
};