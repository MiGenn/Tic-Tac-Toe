#include "InfiniteField.h"
#include "Figure.h"

bool InfiniteField::TryPlaceFigure(Figure figure) noexcept
{
	if (IsPlaceFree(figure.cellIndex))
	{
		m_figures.push_back(figure);
		return true;
	}

	return false;
}

void InfiniteField::Clear() noexcept
{
	m_figures.clear();
}

std::pair<bool, Vector2i> InfiniteField::TryGetCellIndex(Vector2f worldPointInCell) const noexcept
{
	const auto relativePosition{ worldPointInCell - centerPosition };
	Vector2f cellIndex(relativePosition.x / cellSize.x, relativePosition.y / cellSize.y);
	if (cellIndex.x < 0.f)
		cellIndex.x -= 1.f;

	if (cellIndex.y < 0.f)
		cellIndex.y -= 1.f;

	return { true, { (int)cellIndex.x, (int)cellIndex.y } };
}

Box2f InfiniteField::GetBoundingBox() const noexcept
{
	static constexpr Box2f boundingbox{ -FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX };
	return boundingbox;
}

Vector2f InfiniteField::GetCellCenterPosition(Vector2i cellIndex) const noexcept
{
	const auto cellBottomLeft{ GetCellBottomLeft(cellIndex) };
	const auto halfCellSize{ cellSize * 0.5f };

	return cellBottomLeft + halfCellSize;
}

Box2f InfiniteField::GetCellBoundingBox(Vector2i cellIndex) const noexcept
{
	const auto cellBottomLeft{ GetCellBottomLeft(cellIndex) };
	return { cellBottomLeft.x, cellBottomLeft.y + cellSize.y, cellBottomLeft.x + cellSize.x, cellBottomLeft.y };
}

Box2f InfiniteField::GetFigureBoundingBox(Vector2i figureCellIndex) const noexcept
{
	static constexpr auto halfFigureSize{ Figure::figureSize * 0.5f };
	const auto figureCenterPosition{ GetCellCenterPosition(figureCellIndex) };

	return { figureCenterPosition.x - halfFigureSize.x, figureCenterPosition.y + halfFigureSize.y,
		figureCenterPosition.x + halfFigureSize.x, figureCenterPosition.y - halfFigureSize.y };
}

const std::vector<Figure>& InfiniteField::GetFigures() const noexcept
{
	return m_figures;
}

bool InfiniteField::IsPlaceFree(Vector2i cellIndex) const noexcept
{
	for (auto figure : m_figures)
		if (figure.cellIndex == cellIndex)
			return false;

	return true;
}

Vector2f InfiniteField::GetCellBottomLeft(Vector2i cellIndex) const noexcept
{
	return { Vector2f(cellIndex.x * cellSize.x, cellIndex.y * cellSize.y) + centerPosition };
}
