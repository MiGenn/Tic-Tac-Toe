#pragma once
#include "IField.h"
#include "Box2D.h"

class IRenderableField : public IField
{
public:
	static inline constexpr Vector2f centerPosition{ 0.f, 0.f };
	static inline constexpr Vector2f cellSize{ 1.f, 1.f };

	IRenderableField() noexcept = default;
	IRenderableField(const IRenderableField&) = delete;
	virtual ~IRenderableField() noexcept = default;

	IRenderableField& operator=(const IRenderableField&) = delete;

	virtual Box2f GetBoundingBox() const noexcept = 0;
	virtual Vector2f GetCellCenterPosition(Vector2i cellIndex) const noexcept = 0;
	virtual Box2f GetCellBoundingBox(Vector2i cellIndex) const noexcept = 0;
	virtual Box2f GetFigureBoundingBox(Vector2i figureCellIndex) const noexcept = 0;
};