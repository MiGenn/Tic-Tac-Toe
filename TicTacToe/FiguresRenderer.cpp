#include "FiguresRenderer.h"

FiguresRenderer::FiguresRenderer(const TicTacToe& gameApplication,
	GameWindow& gameWindow) noexcept : Renderer(gameWindow),
	m_gameApplication(gameApplication)
{

}

void FiguresRenderer::Render()
{
	const auto crossColor{ GetFigureColor(Figure::Type::Cross) };
	const auto noughtColor{ GetFigureColor(Figure::Type::Nought) };
	const auto figureScreenThickness{ (int)std::round(m_camera.ConvertWorldToScreenThickness(m_figureWorldThickness)) };

	if (figureScreenThickness > 0)
	{
		for (const auto figure : GameWorld::GetField().GetFigures())
		{
			const auto figureScreenBoundingBox{ GetFigureScreenBoundingBox(figure.cellIndex) };
			if (figure.figureType == Figure::Type::Cross)
			{
				RenderCrossShadow(figureScreenBoundingBox, figureScreenThickness);
				RenderCross(figureScreenBoundingBox, figureScreenThickness, crossColor);
			}
			else
			{
				RenderNoughtShadow(figureScreenBoundingBox, figureScreenThickness);
				RenderNought(figureScreenBoundingBox, figureScreenThickness, noughtColor);
			}
		}
	}
}

ColorRGB FiguresRenderer::GetFigureColor(Figure::Type figureType) const noexcept
{
	ColorRGB figureColor;
	if (GameWorld::GetFirstPlayerFigureType() == figureType)
		figureColor = m_gameApplication.firstPlayer.figureColor;
	else
		figureColor = m_gameApplication.secondPlayer.figureColor;

	return figureColor;
}

Box2i FiguresRenderer::GetFigureScreenBoundingBox(Vector2i cellIndex) const noexcept
{
	auto figureBoundingBox = GameWorld::GetField().GetFigureBoundingBox(cellIndex);
	return m_camera.ConvertWorldToScreenBox(figureBoundingBox);
}

void FiguresRenderer::RenderCross(const Box2i& screenBoundingBox, int screenThickness, ColorRGB color) const
{
	m_graphics.RenderLine(screenBoundingBox.topLeft, screenBoundingBox.bottomRight, screenThickness, color);
	m_graphics.RenderLine(screenBoundingBox.GetTopRight(), screenBoundingBox.GetBottomLeft(), screenThickness, color);
}

void FiguresRenderer::RenderNought(const Box2i& screenBoundingBox, int screenThickness, ColorRGB color) const
{
	m_graphics.RenderEllipse(screenBoundingBox, screenThickness, color);
}

void FiguresRenderer::RenderCrossShadow(const Box2i& crossScreenBoundingBox, int screenThickness) const
{
	const auto shadowBoundingBox{ CalculateShadowScreenBoundingBox(crossScreenBoundingBox) };
	const auto shadowBlendedColor{ m_gameApplication.backgroundColor.Blend(m_shadowColor, m_shadowTransparency) };

	m_graphics.RenderLine(shadowBoundingBox.topLeft, shadowBoundingBox.bottomRight, screenThickness, shadowBlendedColor);
	m_graphics.RenderLine(shadowBoundingBox.GetTopRight(), shadowBoundingBox.GetBottomLeft(), screenThickness, shadowBlendedColor);
}

void FiguresRenderer::RenderNoughtShadow(const Box2i& noughtScreenBoundingBox, int screenThickness) const
{
	const auto shadowBoundingBox{ CalculateShadowScreenBoundingBox(noughtScreenBoundingBox) };
	const auto shadowBlendedColor{ m_gameApplication.backgroundColor.Blend(m_shadowColor, m_shadowTransparency) };

	m_graphics.RenderEllipse(shadowBoundingBox, screenThickness, shadowBlendedColor);
}

Box2i FiguresRenderer::CalculateShadowScreenBoundingBox(const Box2i& figureScreenBoundingBox) const
{
	const auto shadowScreenOffset{ (Vector2i)m_camera.ConvertWorldToScreenUnits(m_shadowWorldOffset) };
	return { figureScreenBoundingBox.topLeft + shadowScreenOffset, figureScreenBoundingBox.bottomRight + shadowScreenOffset };
}
