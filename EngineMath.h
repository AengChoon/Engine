#pragma once
#include <cmath>

namespace Math
{
	constexpr float PI = 3.14159265f;

	template <typename T>
	constexpr auto Square(const T& InNumber)
	{
		return InNumber * InNumber;
	}

	template<typename T>
	T WrapAngle(T InRadian)
	{
		const T Modded = fmod(InRadian, static_cast<T>(2.0) * static_cast<T>(PI));
		return Modded > static_cast<T>(PI)
							? Modded - static_cast<T>(2.0) * static_cast<T>(PI)
							: Modded;
	}

	template<typename T>
	constexpr T Interpolate(const T& InSource, const T& InDestination, float InAlpha)
	{
		return InSource + (InDestination - InSource) * InAlpha;
	}

	template<typename T>
	constexpr T DegreeToRadian(T InDegree)
	{
		return InDegree * PI / static_cast<T>(180.0);
	}
}

