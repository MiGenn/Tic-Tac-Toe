#pragma once
#include "Script.h"
#include "Figure.h"
#include "GameWindow.h"
#include "TicTacToe.h"
#include "GameWorld.h"

class FigurePlacerScript final : public Script
{
public:
	FigurePlacerScript(TicTacToe& gameApplication, ConfigurationWindow& configurationWindow, GameWindow& gameWindow) noexcept;
	FigurePlacerScript(const FigurePlacerScript&) = delete;

	FigurePlacerScript& operator=(const FigurePlacerScript&) = delete;

	virtual void Execute() override;

private:
	const std::wstring m_sessionHistoryFullPath;

	TicTacToe& m_gameApplication;
	GameWindow& m_gameWindow;
	ConfigurationWindow& m_configurationWindow;

	void PlaceCurrentPlayerFigureOrNot(Vector2i cellIndex) const;
	bool IsGameOver(Figure figure) const;

	void SaveSessionInfo(const PlayerNonGameData& firstPlayerData, 
		const PlayerNonGameData& secondPlayerData, GameWorld::Player winner) const;
	SYSTEMTIME GetCurrentSystemTime() const noexcept;

	void ShowWinner(const PlayerNonGameData& winner) const noexcept;
};
