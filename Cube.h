#pragma once
#include "IndexedTriangleList.h"

class Cube
{
public:
	template<typename T>
	static IndexedTriangleList<T> Make()
	{
		constexpr float EdgeLength = 1.0f / 2.0f;

		std::vector<T> Vertices {8};
		Vertices[0].Position = {-EdgeLength,-EdgeLength, -EdgeLength};
		Vertices[1].Position = {EdgeLength,-EdgeLength, -EdgeLength};
		Vertices[2].Position = {-EdgeLength,EdgeLength, -EdgeLength};
		Vertices[3].Position = {EdgeLength,EdgeLength, -EdgeLength};
		Vertices[4].Position = {-EdgeLength,-EdgeLength, EdgeLength};
		Vertices[5].Position = {EdgeLength,-EdgeLength, EdgeLength};
		Vertices[6].Position = {-EdgeLength,EdgeLength, EdgeLength};
		Vertices[7].Position = {EdgeLength, EdgeLength, EdgeLength};

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

	template<typename T>
	static IndexedTriangleList<T> MakeTextured()
	{
		constexpr float EdgeLength = 1.0f / 2.0f;

		std::vector<T> Vertices {14};
		Vertices[0].Position = {-EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[0].Texture  = {2.0f / 3.0f, 0.0f / 4.0f};

		Vertices[1].Position = {EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[1].Texture  = {1.0f / 3.0f, 0.0f / 4.0f};

		Vertices[2].Position = {-EdgeLength, EdgeLength, -EdgeLength};
		Vertices[2].Texture  = {2.0f / 3.0f, 1.0f / 4.0f};

		Vertices[3].Position = {EdgeLength, EdgeLength, -EdgeLength};
		Vertices[3].Texture  = {1.0f / 3.0f, 1.0f / 4.0f};

		Vertices[4].Position = {-EdgeLength, -EdgeLength, EdgeLength};
		Vertices[4].Texture  = {2.0f / 3.0f, 3.0f / 4.0f};

		Vertices[5].Position = {EdgeLength, -EdgeLength, EdgeLength};
		Vertices[5].Texture  = {1.0f / 3.0f, 3.0f / 4.0f};

		Vertices[6].Position = {-EdgeLength, EdgeLength, EdgeLength};
		Vertices[6].Texture  = {2.0f / 3.0f, 2.0f / 4.0f};

		Vertices[7].Position = {EdgeLength, EdgeLength, EdgeLength};
		Vertices[7].Texture  = {1.0f / 3.0f, 2.0f / 4.0f};

		Vertices[8].Position = {-EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[8].Texture  = {2.0f / 3.0f, 4.0f / 4.0f};

		Vertices[9].Position = {EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[9].Texture  = {1.0f / 3.0f, 4.0f / 4.0f};

		Vertices[10].Position = {-EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[10].Texture  = {3.0f / 3.0f, 1.0f / 4.0f};

		Vertices[11].Position = {-EdgeLength, -EdgeLength, EdgeLength};
		Vertices[11].Texture  = {3.0f / 3.0f, 2.0f / 4.0f};

		Vertices[12].Position = {EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[12].Texture  = {0.0f / 3.0f, 1.0f / 4.0f};

		Vertices[13].Position = {EdgeLength, -EdgeLength, EdgeLength};
		Vertices[13].Texture  = {0.0f / 3.0f, 2.0f / 4.0f};

		return {
					std::move(Vertices),
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

	template<typename T>
	static IndexedTriangleList<T> MakeIndependent()
	{
		constexpr float EdgeLength = 1.0f / 2.0f;

		std::vector<T> Vertices {24};
		Vertices[0].Position = {-EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[1].Position = {EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[2].Position = {-EdgeLength, EdgeLength, -EdgeLength};
		Vertices[3].Position = {EdgeLength, EdgeLength, -EdgeLength};
		Vertices[4].Position = {-EdgeLength, -EdgeLength, EdgeLength};
		Vertices[5].Position = {EdgeLength, -EdgeLength, EdgeLength};
		Vertices[6].Position = {-EdgeLength, EdgeLength, EdgeLength};
		Vertices[7].Position = {EdgeLength, EdgeLength, EdgeLength};
		Vertices[8].Position = {-EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[9].Position = {-EdgeLength, EdgeLength, -EdgeLength};
		Vertices[10].Position = {-EdgeLength, -EdgeLength, EdgeLength};
		Vertices[11].Position = {-EdgeLength, EdgeLength, EdgeLength};
		Vertices[12].Position = {EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[13].Position = {EdgeLength, EdgeLength, -EdgeLength};
		Vertices[14].Position = {EdgeLength, -EdgeLength, EdgeLength};
		Vertices[15].Position = {EdgeLength, EdgeLength, EdgeLength};
		Vertices[16].Position = {-EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[17].Position = {EdgeLength, -EdgeLength, -EdgeLength};
		Vertices[18].Position = {-EdgeLength, -EdgeLength, EdgeLength};
		Vertices[19].Position = {EdgeLength, -EdgeLength, EdgeLength};
		Vertices[20].Position = {-EdgeLength, EdgeLength, -EdgeLength};
		Vertices[21].Position = {EdgeLength, EdgeLength, -EdgeLength};
		Vertices[22].Position = {-EdgeLength, EdgeLength, EdgeLength};
		Vertices[23].Position = {EdgeLength, EdgeLength, EdgeLength};

		return {
					std::move(Vertices),
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