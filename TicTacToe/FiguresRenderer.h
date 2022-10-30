#pragma once
#include "Renderer.h"
#include "GameWorld.h"
#include "TicTacToe.h"
#include "IRenderableField.h"

class FiguresRenderer final : public Renderer
{
public:
	FiguresRenderer(const TicTacToe& gameApplication, GameWindow& gameWindow) noexcept;
	FiguresRenderer(const FiguresRenderer&) = delete;

	FiguresRenderer& operator=(const FiguresRenderer&) = delete;

	virtual void Render() override;

private:
	static constexpr auto m_figureWorldThickness{ std::min(Figure::figureSize.x * 0.21f, Figure::figureSize.y * 0.21f) };

	static inline constexpr ColorRGB m_shadowColor{ 0, 0, 0 };
	static constexpr unsigned char m_shadowTransparency{ 85 };
	static constexpr Vector2f m_shadowWorldOffset{ Figure::figureSize.x * 0.23f, Figure::figureSize.y * 0.23f };

	const TicTacToe& m_gameApplication;

	ColorRGB GetFigureColor(Figure::Type figureType) const noexcept;
	Box2i GetFigureScreenBoundingBox(Vector2i cellIndex) const noexcept;

	void RenderCross(const Box2i& screenBoundingBox, int screenThickness, ColorRGB color) const;
	void RenderNought(const Box2i& screenBoundingBox, int screenThickness, ColorRGB color) const;

	void RenderCrossShadow(const Box2i& crossScreenBoundingBox, int screenThickness) const;
	void RenderNoughtShadow(const Box2i& noughtScreenBoundingBox, int screenThickness) const;
	Box2i CalculateShadowScreenBoundingBox(const Box2i& figureScreenBoundingBox) const;
};
