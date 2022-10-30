#pragma once
#include "Concepts.h"

template<Number T>
inline constexpr bool HaveDifferentSigns(T a, T b) noexcept
{
	return a < T() != b < T();
}

template<Number T>
inline constexpr bool HaveSameSign(T a, T b) noexcept
{
	return !HaveDifferentSigns(a, b);
}
