#pragma once
#include "Renderer.h"
#include "TicTacToe.h"
#include "Figure.h"
#include "Text.h"

class TurnInfoRenderer final : public Renderer
{
public:
	TurnInfoRenderer(const TicTacToe& gameApplication, GameWindow& gameWindow) noexcept;
	TurnInfoRenderer(const TurnInfoRenderer&) = delete;

	TurnInfoRenderer& operator=(const TurnInfoRenderer&) = delete;

	virtual void Render() override;

private:
	const TicTacToe& m_gameApplication;

	Text m_text;
	static inline const std::wstring m_crossName{ L"Cross" };
	static inline const std::wstring m_noughtName{ L"Nought" };

	const std::wstring& FigureTypeToString(Figure::Type figureType) const noexcept;
};
