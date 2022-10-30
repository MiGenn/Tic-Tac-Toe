#pragma once
#include "IRenderableField.h"

class Figure final
{
public:
	enum class Type
	{
		Cross,
		Nought
	};

	static inline constexpr Vector2f figureSize{ IRenderableField::cellSize * 0.42f };

	Type figureType;
	Vector2i cellIndex;

	Figure() noexcept = default;
	Figure(Type figureType, Vector2i cellIndex) noexcept;
};
