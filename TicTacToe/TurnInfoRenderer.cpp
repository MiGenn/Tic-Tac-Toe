#include "TurnInfoRenderer.h"
#include "GameWorld.h"
#include <format>

TurnInfoRenderer::TurnInfoRenderer(const TicTacToe& gameApplication, 
	GameWindow& gameWindow) noexcept : Renderer(gameWindow),
	m_gameApplication(gameApplication)
{
	m_text.SetFont(L"UNISPACE");
	m_text.color = { 255, 255, 255 };
}

void TurnInfoRenderer::Render()
{
	static constexpr TextFormat textFormat{ TextFormat::AlignmentX::Left, 
		TextFormat::AlignmentY::Top, TextFormat::Overflow::Ellipsis };
	const Box2i textBoundingBox{ 5, 5, m_graphics.GetCanvasSize().x, 40 };

	const auto& currentPlayerName{ m_gameApplication.GetCurrentPlayer().name };
	const auto& currentPlayerFigureType{ FigureTypeToString(GameWorld::GetCurrentPlayerFigureType()) };

	m_text.text = std::format(L"{} turn ({})", currentPlayerName, currentPlayerFigureType);
	m_graphics.RenderText(textBoundingBox, m_text, textFormat);
}

const std::wstring& TurnInfoRenderer::FigureTypeToString(Figure::Type figureType) const noexcept
{
	if (figureType == Figure::Type::Cross)
		return m_crossName;

	return m_noughtName;
}
