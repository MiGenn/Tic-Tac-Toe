#pragma once
#include "WinapiException.h"
#include "Vector2D.h"
#include "Box2D.h"
#include "ColorRGB.h"

constexpr Vector2i PointsToVector(POINTS point) noexcept
{
	return { point.x, point.y };
}

constexpr Vector2i PointToVector(POINT point) noexcept
{
	return { point.x, point.y };
}

constexpr POINT VectorToPoint(Vector2i vector) noexcept
{
	return { vector.x, vector.y };
}

constexpr Box2i RectToBox(const RECT& rect) noexcept
{
	return { rect.left, rect.top, rect.right, rect.bottom };
}

constexpr RECT BoxToRect(const Box2i& box) noexcept
{
	return { box.GetLeft(), box.GetTop(), box.GetRight(), box.GetBottom() };
}

constexpr ColorRGB ColorrefToColorRGB(COLORREF colorref) noexcept
{
	return { GetRValue(colorref), GetGValue(colorref), GetBValue(colorref) };
}

constexpr COLORREF ColorRGBToColorref(ColorRGB rgb) noexcept
{
	return RGB(rgb.red, rgb.green, rgb.blue);
}
