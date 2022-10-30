#pragma once
#include "Game.h"
#include "ConfigurationWindow.h"
#include "PlayerNonGameData.h"

class TicTacToe final : public Game
{
public:
	static inline const std::wstring sessionHistoryFileName{ L"SessionHistory.txt" };

	PlayerNonGameData firstPlayer{ L"Player 1", { 254, 208, 0 } };
	PlayerNonGameData secondPlayer{ L"Player 2", { 255, 255, 255 } };

	ColorRGB backgroundColor{ 235, 75, 51 };

	TicTacToe();
	TicTacToe(const TicTacToe&) = delete;

	TicTacToe& operator=(const TicTacToe&) = delete;

	const PlayerNonGameData& GetCurrentPlayer() const noexcept;
	const PlayerNonGameData& GetNextPlayer() const noexcept;

private:
	ConfigurationWindow m_configurationWindow;

	void Configure();
	void LoadScripts();
};
