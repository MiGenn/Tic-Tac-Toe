#pragma once
#include "IRenderableField.h"

class InfiniteField final : public IRenderableField
{
public:
	InfiniteField() noexcept = default;
	InfiniteField(const InfiniteField&) = delete;

	InfiniteField& operator=(const InfiniteField&) = delete;

	virtual bool TryPlaceFigure(Figure figure) noexcept override;
	virtual void Clear() noexcept;

	virtual std::pair<bool, Vector2i> TryGetCellIndex(Vector2f worldPointInCell) const noexcept override;
	virtual const std::vector<Figure>& GetFigures() const noexcept;

	virtual Box2f GetBoundingBox() const noexcept override;
	virtual Vector2f GetCellCenterPosition(Vector2i cellIndex) const noexcept override;
	virtual Box2f GetCellBoundingBox(Vector2i cellIndex) const noexcept override;
	virtual Box2f GetFigureBoundingBox(Vector2i figureCellIndex) const noexcept override;

private:
	std::vector<Figure> m_figures;

	bool IsPlaceFree(Vector2i position) const noexcept;
	Vector2f GetCellBottomLeft(Vector2i cellIndex) const noexcept;
};
