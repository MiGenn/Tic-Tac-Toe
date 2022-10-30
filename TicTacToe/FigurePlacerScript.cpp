#include "FigurePlacerScript.h"
#include "CombinationChecker.h"
#include "TicTacToe.h"
#include "SessionInfoSaver.h"
#include "PlayerNonGameData.h"

FigurePlacerScript::FigurePlacerScript(TicTacToe& gameApplication, 
	ConfigurationWindow& configurationWindow, GameWindow& gameWindow) noexcept : Script(gameWindow),
	m_gameApplication(gameApplication), m_gameWindow(gameWindow), m_configurationWindow(configurationWindow),
	m_sessionHistoryFullPath(gameApplication.directory + gameApplication.sessionHistoryFileName)
{
	
}

void FigurePlacerScript::Execute()
{
	if (m_mouse.IsButtonPressed(Mouse::Button::Left))
	{
		const auto mouseWorldPosition{ m_camera.ConvertScreenToWorldPoint(m_mouse.GetPosition()) };
		const auto [isCellIndexValid, correspondingCellIndex] { GameWorld::GetField().TryGetCellIndex(mouseWorldPosition) };

		if (isCellIndexValid)
			PlaceCurrentPlayerFigureOrNot(correspondingCellIndex);
	}
}

void FigurePlacerScript::PlaceCurrentPlayerFigureOrNot(Vector2i cellIndex) const
{
	const auto currentPlayer{ GameWorld::GetCurrentPlayer() };
	const auto currentPlayerFigureType{ GameWorld::GetCurrentPlayerFigureType() };
	const auto currentPlayerData{ m_gameApplication.GetCurrentPlayer() };

	if (GameWorld::TryPlaceCurrentPlayerFigure(cellIndex))
	{
		const Figure placedFigure(currentPlayerFigureType, cellIndex);
		if (IsGameOver(placedFigure))
		{
			SaveSessionInfo(m_gameApplication.firstPlayer, m_gameApplication.secondPlayer, currentPlayer);
			ShowWinner(currentPlayerData);
			m_configurationWindow.StartConfiguration();
		}
	}
}

bool FigurePlacerScript::IsGameOver(Figure figure) const
{
	return CombinationChecker::IsThereWinningCombinationForFigure(figure, GameWorld::GetField(), GameWorld::GetWinningCombinationLength());
}

void FigurePlacerScript::SaveSessionInfo(const PlayerNonGameData& firstPlayerData,
	const PlayerNonGameData& secondPlayerData, GameWorld::Player winner) const
{
	const auto endTime{ GetCurrentSystemTime() };
	std::wofstream file(m_sessionHistoryFullPath, std::ios::app);

	SessionInfoSaver::Save(file, endTime, firstPlayerData, secondPlayerData, winner);
}

SYSTEMTIME FigurePlacerScript::GetCurrentSystemTime() const noexcept
{
	SYSTEMTIME currentTime{};
	GetLocalTime(&currentTime);

	return currentTime;
}

void FigurePlacerScript::ShowWinner(const PlayerNonGameData& winner) const noexcept
{
	m_gameWindow.Deactivate();
	m_gameWindow.Rerender();

	const auto message{ winner.name + L" is winner" };
	MessageBox(nullptr, message.c_str(), L"Game over", MB_OK );
}
