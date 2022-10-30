#include "FieldRenderer.h"
#include "GameWorld.h"
#include <cmath>

FieldRenderer::FieldRenderer(const TicTacToe& gameApplication, GameWindow& gameWindow) noexcept : Renderer(gameWindow),
	m_gameApplication(gameApplication)
{

}

void FieldRenderer::Render()
{
	const auto gridWorldBoundingBox{ CalculateGridWorldBoundingBox() };
	const auto gridScreenThickness{ (int)std::round(m_camera.ConvertWorldToScreenThickness(m_gridWorldThickness)) };

	if (gridWorldBoundingBox.topLeft != gridWorldBoundingBox.bottomRight ||
		gridScreenThickness > 0)
	{
		RenderGridShadow(gridWorldBoundingBox, gridScreenThickness);
		RenderGrid(gridWorldBoundingBox, gridScreenThickness);
	}
}

Box2f FieldRenderer::CalculateGridWorldBoundingBox() const
{
	static constexpr Vector2f intersectionBoxOffset{ -m_epsilon, m_epsilon };

	const auto& field{ GameWorld::GetField() };

	const Box2f viewedWorldAreaBox{ m_camera.ConvertScreenToWorldBox({ { 0.f, 0.f }, m_graphics.GetCanvasSize() }) };
	auto [isIntersection, intersectionBox] { viewedWorldAreaBox.TryGetIntersectionBox(field.GetBoundingBox()) };
	intersectionBox.topLeft -= intersectionBoxOffset;
	intersectionBox.bottomRight += intersectionBoxOffset;

	if (!isIntersection)
		return {};
	
	const auto [isTopLeftCellIndexValid, topLeftCellIndex] { field.TryGetCellIndex(intersectionBox.topLeft) };
	const auto [isBottomRightCellIndexValid, bottomRightCellIndex] { field.TryGetCellIndex(intersectionBox.bottomRight) };

	if (!isTopLeftCellIndexValid || !isBottomRightCellIndexValid)
		throw std::logic_error("Cell indices must be valid");

	return { field.GetCellBoundingBox(topLeftCellIndex).topLeft, field.GetCellBoundingBox(bottomRightCellIndex).bottomRight };
}

void FieldRenderer::RenderGrid(const Box2f& worldBoundingBox, int screenThickness) const
{
	RenderHorizontalLines(worldBoundingBox, screenThickness, m_gridColor);
	RenderVerticalLines(worldBoundingBox, screenThickness, m_gridColor);
}

void FieldRenderer::RenderHorizontalLines(const Box2f& worldBoundingBox, int screenThickness, ColorRGB color) const
{
	const auto startX{ worldBoundingBox.GetLeft() };
	const auto endX{ worldBoundingBox.GetRight() };

	const auto startY{ worldBoundingBox.GetTop() };
	const auto endY{ worldBoundingBox.GetBottom() - m_epsilon };

	for (auto y{ startY }; y >= endY; y -= IRenderableField::cellSize.y)
	{
		const auto startPosition{ m_camera.ConvertWorldToScreenPoint({ startX, y }) };
		const auto endPosition{ m_camera.ConvertWorldToScreenPoint({ endX, y }) };

		m_graphics.RenderLine(startPosition, endPosition, screenThickness, color);
	}
}

void FieldRenderer::RenderVerticalLines(const Box2f& worldBoundingBox, int screenThickness, ColorRGB color) const
{
	const auto startY{ worldBoundingBox.GetTop() };
	const auto endY{ worldBoundingBox.GetBottom() };

	const auto startX{ worldBoundingBox.GetLeft() };
	const auto endX{ worldBoundingBox.GetRight() + m_epsilon };

	for (auto x{ startX }; x <= endX; x += IRenderableField::cellSize.x)
	{
		const auto startPosition{ m_camera.ConvertWorldToScreenPoint({ x, startY }) };
		const auto endPosition{ m_camera.ConvertWorldToScreenPoint({ x, endY }) };

		m_graphics.RenderLine(startPosition, endPosition, screenThickness, color);
	}
}

void FieldRenderer::RenderGridShadow(const Box2f& gridWorldBoundingBox, int screenThickness) const
{
	const Box2f shadowWorldBoundingBox{ gridWorldBoundingBox.topLeft + m_shadowWorldOffset,
		gridWorldBoundingBox.bottomRight + m_shadowWorldOffset };
	const auto shadowBlendedColor{ m_gameApplication.backgroundColor.Blend(m_shadowColor, m_shadowTransparency) };

	RenderHorizontalLines(shadowWorldBoundingBox, screenThickness, shadowBlendedColor);
	RenderVerticalLines(shadowWorldBoundingBox, screenThickness, shadowBlendedColor);
}
