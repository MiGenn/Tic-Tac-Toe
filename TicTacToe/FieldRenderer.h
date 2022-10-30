#pragma once
#include "Renderer.h"
#include "TicTacToe.h"
#include "IRenderableField.h"

class FieldRenderer final : public Renderer
{
public:
	FieldRenderer(const TicTacToe& gameApplication, GameWindow& gameWindow) noexcept;
	FieldRenderer(const FieldRenderer&) = delete;

	FieldRenderer& operator=(const FieldRenderer&) = delete;

	virtual void Render() override;

private:
	static constexpr float m_epsilon{ 0.01f }; // Потрібен через неточності в розрахунках

	static constexpr float m_gridWorldThickness{ 
		std::min(IRenderableField::cellSize.x * 0.23f, IRenderableField::cellSize.y * 0.23f) };
	static inline constexpr ColorRGB m_gridColor{ 29, 54, 76 };

	static inline constexpr Vector2f m_shadowWorldOffset{ 
		IRenderableField::cellSize.x * 0.085f, -IRenderableField::cellSize.y * 0.085f };
	static inline constexpr ColorRGB m_shadowColor{ 0, 0, 0 };
	static constexpr auto m_shadowTransparency{ 85 };

	const TicTacToe& m_gameApplication;

	Box2f CalculateGridWorldBoundingBox() const;

	void RenderGrid(const Box2f& gridWorldBoundingBox, int screenThickness) const;
	void RenderHorizontalLines(const Box2f& worldBoundingBox, int screenThickness, ColorRGB color) const;
	void RenderVerticalLines(const Box2f& worldBoundingBox, int screenThickness, ColorRGB color) const;

	void RenderGridShadow(const Box2f& gridWorldBoundingBox, int screenThickness) const;
};
