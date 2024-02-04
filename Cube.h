#pragma once
#include "IndexedTriangleList.h"

class Cube
{
public:
	static IndexedTriangleList Make(DV::VertexLayout InLayout = {})
	{
		if (!InLayout.Size())
		{
			InLayout.Append(DV::VertexLayout::ElementType::Position3D);
		}

		constexpr float EdgeLength = 1.0f / 2.0f;

		DV::VertexBuffer CubeVertexBuffer {std::move(InLayout)};
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, -EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, -EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, EdgeLength, EdgeLength});

		return {
					std::move(CubeVertexBuffer),
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

	static IndexedTriangleList MakeTextured(DV::VertexLayout InLayout = {})
	{
		if (!InLayout.Size())
		{
			InLayout.Append(DV::VertexLayout::ElementType::Position3D).Append(DV::VertexLayout::ElementType::Texture2D);
		}

		constexpr float EdgeLength = 1.0f / 2.0f;

		DV::VertexBuffer CubeVertexBuffer {std::move(InLayout)};

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, -EdgeLength},
			DirectX::XMFLOAT2{2.0f / 3.0f, 0.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{EdgeLength, -EdgeLength, -EdgeLength},
			DirectX::XMFLOAT2{1.0f / 3.0f, 0.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{-EdgeLength, EdgeLength, -EdgeLength},
			DirectX::XMFLOAT2{2.0f / 3.0f, 1.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{EdgeLength, EdgeLength, -EdgeLength},
			DirectX::XMFLOAT2{1.0f / 3.0f, 1.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, EdgeLength},
			DirectX::XMFLOAT2{2.0f / 3.0f, 3.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{EdgeLength, -EdgeLength, EdgeLength},
			DirectX::XMFLOAT2{1.0f / 3.0f, 3.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{-EdgeLength, EdgeLength, EdgeLength},
			DirectX::XMFLOAT2{2.0f / 3.0f, 2.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{EdgeLength, EdgeLength, EdgeLength},
			DirectX::XMFLOAT2{1.0f / 3.0f, 2.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, -EdgeLength},
			DirectX::XMFLOAT2{2.0f / 3.0f, 4.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{EdgeLength, -EdgeLength, -EdgeLength},
			DirectX::XMFLOAT2{1.0f / 3.0f, 4.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, -EdgeLength},
			DirectX::XMFLOAT2{3.0f / 3.0f, 1.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, EdgeLength},
			DirectX::XMFLOAT2{3.0f / 3.0f, 2.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{EdgeLength, -EdgeLength, -EdgeLength},
			DirectX::XMFLOAT2{0.0f / 3.0f, 1.0f / 4.0f}
		);

		CubeVertexBuffer.Emplace
		(
			DirectX::XMFLOAT3{EdgeLength, -EdgeLength, EdgeLength},
			DirectX::XMFLOAT2{0.0f / 3.0f, 2.0f / 4.0f}
		);

		return {
					std::move(CubeVertexBuffer),
					{
						0,2,1,   2,3,1,
						4,8,5,   5,8,9,
						2,6,3,   3,6,7,
						4,5,7,   4,7,6,
						2,10,11, 2,11,6,
						12,3,7,  12,7,13
					}
				};
	}

	static IndexedTriangleList MakeIndependent(DV::VertexLayout InLayout = {})
	{
		if (!InLayout.Size())
		{
			InLayout.Append(DV::VertexLayout::ElementType::Position3D);
		}

		constexpr float EdgeLength = 1.0f / 2.0f;

		DV::VertexBuffer CubeVertexBuffer {std::move(InLayout)};

		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, -EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, -EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, -EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, -EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, -EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, -EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, -EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, EdgeLength, -EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{-EdgeLength, EdgeLength, EdgeLength});
		CubeVertexBuffer.Emplace(DirectX::XMFLOAT3{EdgeLength, EdgeLength, EdgeLength});

		return {
					std::move(CubeVertexBuffer),
					{
						0,2,1,    2,3,1,
						4,5,7,    4,7,6,
						8,10,9,   10,11,9,
						12,13,15, 12,15,14,
						16,17,18, 18,17,19,
						20,23,21, 20,22,23
					}
				};
	}
};