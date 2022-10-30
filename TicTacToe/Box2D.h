#pragma once
#include "Vector2D.h"

template<Number T>
class Box2D final
{
public:	
	Vector2D<T> topLeft;
	Vector2D<T> bottomRight;

	constexpr Box2D<T>() noexcept = default;
	constexpr Box2D<T>(Vector2D<T> topLeft, Vector2D<T> bottomRight) noexcept;
	constexpr Box2D<T>(T left, T top, T right, T bottom) noexcept;

	template<Number OutputT>
	constexpr operator Box2D<OutputT>() const noexcept;

	constexpr bool IsPointInBox(Vector2D<T> point) const noexcept;
	constexpr bool IsIntersection(const Box2D<T>& box) const noexcept;
	constexpr std::pair<bool, Box2D<T>> TryGetIntersectionBox(const Box2D<T>& box) const noexcept;

	constexpr T GetTop() const noexcept;
	constexpr T GetLeft() const noexcept;
	constexpr T GetBottom() const noexcept;
	constexpr T GetRight() const noexcept;

	constexpr Vector2D<T> GetTopRight() const noexcept;
	constexpr Vector2D<T> GetBottomLeft() const noexcept;

	constexpr Vector2D<T> GetCenter() const noexcept;
};

using Box2f = Box2D<float>;
using Box2i = Box2D<int>;

template<Number T>
inline constexpr Box2D<T>::Box2D(Vector2D<T> topLeft, Vector2D<T> bottomRight) noexcept :
	topLeft(topLeft), bottomRight(bottomRight)
{

}

template<Number T>
inline constexpr Box2D<T>::Box2D(T left, T top, T right, T bottom) noexcept :
	topLeft(left, top), bottomRight(right, bottom)
{

}

template<Number T>
template<Number OutputT>
inline constexpr Box2D<T>::operator Box2D<OutputT>() const noexcept
{
	return { (Vector2D<T>)topLeft, (Vector2D<T>)bottomRight };
}

template<Number T>
inline constexpr bool Box2D<T>::IsPointInBox(Vector2D<T> point) const noexcept
{
	return point.x >= topLeft.x && point.x <= bottomRight.x &&
		point.y >= topLeft.y && point.y <= bottomRight.y;
}

template<Number T>
inline constexpr bool Box2D<T>::IsIntersection(const Box2D<T>& box) const noexcept
{
	if (box.topLeft.x > bottomRight.x || box.bottomRight.x < topLeft.x)
		return false;

	if (box.topLeft.y > bottomRight.y || box.bottomRight.y < topLeft.y)
		return false;

	return true;
}

template<Number T>
inline constexpr std::pair<bool, Box2D<T>> Box2D<T>::TryGetIntersectionBox(const Box2D<T>& box) const noexcept
{
	Box2D<T> intersectionBox;

	if (box.topLeft.x <= bottomRight.x && box.topLeft.x >= topLeft.x)
	{
		intersectionBox.topLeft.x = box.topLeft.x;
		intersectionBox.bottomRight.x = bottomRight.x;

		if (box.bottomRight.x < bottomRight.x)
			intersectionBox.bottomRight.x = box.bottomRight.x;
	}
	else if (box.bottomRight.x <= bottomRight.x && box.bottomRight.x >= topLeft.x)
	{
		intersectionBox.bottomRight.x = box.bottomRight.x;
		intersectionBox.topLeft.x = topLeft.x;

		if (box.topLeft.x > topLeft.x)
			intersectionBox.topLeft.x = box.topLeft.x;
	}
	else if (box.topLeft.x < topLeft.x && box.bottomRight.x > bottomRight.x ||
		box.topLeft.x > bottomRight.x && box.bottomRight.x < topLeft.x)
	{
		intersectionBox.topLeft.x = topLeft.x;
		intersectionBox.bottomRight.x = bottomRight.x;
	}
	else
	{
		return {};
	}

	if (box.topLeft.y >= bottomRight.y && box.topLeft.y <= topLeft.y)
	{
		intersectionBox.topLeft.y = box.topLeft.y;
		intersectionBox.bottomRight.y = bottomRight.y;

		if (box.bottomRight.y > bottomRight.y)
			intersectionBox.bottomRight.y = box.bottomRight.y;
	}
	else if (box.bottomRight.y >= bottomRight.y && box.bottomRight.y <= topLeft.y)
	{
		intersectionBox.bottomRight.y = box.bottomRight.y;
		intersectionBox.topLeft.y = topLeft.y;

		if (box.topLeft.y < topLeft.y)
			intersectionBox.topLeft.y = box.topLeft.y;
	}
	else if (box.topLeft.y > topLeft.y && box.bottomRight.y < bottomRight.y ||
		box.topLeft.y < bottomRight.y && box.bottomRight.y > topLeft.y)
	{
		intersectionBox.topLeft.y = topLeft.y;
		intersectionBox.bottomRight.y = bottomRight.y;
	}
	else
	{
		return {};
	}

	return { true, intersectionBox };
}

template<Number T>
inline constexpr T Box2D<T>::GetTop() const noexcept
{
	return topLeft.y;
}

template<Number T>
inline constexpr T Box2D<T>::GetLeft() const noexcept
{
	return topLeft.x;
}

template<Number T>
inline constexpr T Box2D<T>::GetBottom() const noexcept
{
	return bottomRight.y;
}

template<Number T>
inline constexpr T Box2D<T>::GetRight() const noexcept
{
	return bottomRight.x;
}

template<Number T>
inline constexpr Vector2D<T> Box2D<T>::GetTopRight() const noexcept
{
	return { bottomRight.x, topLeft.y };
}

template<Number T>
inline constexpr Vector2D<T> Box2D<T>::GetBottomLeft() const noexcept
{
	return { topLeft.x, bottomRight.y };
}

template<Number T>
inline constexpr Vector2D<T> Box2D<T>::GetCenter() const noexcept
{
	return (topLeft + bottomRight) / 2;
}
