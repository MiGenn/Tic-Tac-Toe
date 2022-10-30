#pragma once
#include "Concepts.h"

template<Number T>
class Vector2D final
{
public:
	T x = T();
	T y = T();

	constexpr Vector2D() noexcept = default;
	constexpr Vector2D(T x, T y) noexcept;

	constexpr bool operator==(const Vector2D<T>& right) const noexcept;
	constexpr Vector2D<T> operator+(const Vector2D<T>& right) const noexcept;
	constexpr Vector2D<T>& operator+=(const Vector2D<T>& right) noexcept;
	constexpr Vector2D<T> operator-(const Vector2D<T>& right) const noexcept;
	constexpr Vector2D<T>& operator-=(const Vector2D<T>& right) noexcept;
	constexpr Vector2D<T> operator-() const noexcept;
	constexpr Vector2D<T> operator*(T right) const noexcept;
	constexpr Vector2D<T> operator/(T right) const;
	template<Number OutputT> 
	constexpr operator Vector2D<OutputT>() const noexcept;
};

using Vector2f = Vector2D<float>;
using Vector2i = Vector2D<int>;

template<Number T>
inline constexpr Vector2D<T>::Vector2D(T x, T y) noexcept
{
	this->x = x;
	this->y = y;
}

template<Number T>
inline constexpr bool Vector2D<T>::operator==(const Vector2D<T>& right) const noexcept
{
	return x == right.x && y == right.y;
}

template<Number T>
inline constexpr Vector2D<T> Vector2D<T>::operator+(const Vector2D<T>& right) const noexcept
{
	return { x + right.x, y + right.y };
}

template<Number T>
inline constexpr Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& right) noexcept
{
	return (*this) = (*this) + right;
}

template<Number T>
inline constexpr Vector2D<T> Vector2D<T>::operator-(const Vector2D<T>& right) const noexcept
{
	return { x - right.x, y - right.y };
}

template<Number T>
inline constexpr Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& right) noexcept
{
	return (*this) = (*this) - right;
}

template<Number T>
inline constexpr Vector2D<T> Vector2D<T>::operator-() const noexcept
{
	return { -x, -y };
}

template<Number T>
inline constexpr Vector2D<T> Vector2D<T>::operator*(T right) const noexcept
{
	return { x * right, y * right};
}

template<Number T>
inline constexpr Vector2D<T> Vector2D<T>::operator/(T right) const
{
	return { x / right, y / right };
}

template<Number T>
template<Number OutputT>
inline constexpr Vector2D<T>::operator Vector2D<OutputT>() const noexcept
{
	return { OutputT(x), OutputT(y) };
}
