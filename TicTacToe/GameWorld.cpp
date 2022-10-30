#include "GameWorld.h"
#include <stdexcept>

std::unique_ptr<IRenderableField> GameWorld::m_field;
bool GameWorld::m_isFirstPlayerTurn{ true };

Figure::Type GameWorld::GetFirstPlayerFigureType() noexcept
{
	return m_firstPlayerFigureType;
}

Figure::Type GameWorld::GetSecondPlayerFigureType() noexcept
{
	const auto firstPlayerFigureCode{ (bool)m_firstPlayerFigureType };
	return Figure::Type(!firstPlayerFigureCode);
}

GameWorld::Player GameWorld::GetCurrentPlayer() noexcept
{
	if (m_isFirstPlayerTurn)
		return Player::FirstPlayer;

	return Player::SecondPlayer;
}

GameWorld::Player GameWorld::GetNextPlayer() noexcept
{
	if (m_isFirstPlayerTurn)
		return Player::SecondPlayer;

	return Player::FirstPlayer;
}

Figure::Type GameWorld::GetCurrentPlayerFigureType() noexcept
{
	if (m_isFirstPlayerTurn)
		return GetFirstPlayerFigureType();

	return GetSecondPlayerFigureType();
}

Figure::Type GameWorld::GetNextPlayerFigureType() noexcept
{
	if (m_isFirstPlayerTurn)
		return GetSecondPlayerFigureType();

	return GetFirstPlayerFigureType();
}

bool GameWorld::TryPlaceCurrentPlayerFigure(Vector2i cellIndex) noexcept
{
	if (m_field->TryPlaceFigure({ GetCurrentPlayerFigureType(), cellIndex }))
	{
		SwitchTurn();
		return true;
	}

	return false;
}

const IRenderableField& GameWorld::GetField() noexcept
{
	return *m_field;
}

int GameWorld::GetWinningCombinationLength() noexcept
{
	return m_winningCombinationLength;
}

void GameWorld::Reset(std::unique_ptr<IRenderableField>&& field, int winningCombinationLength, bool isFirstPlayerTurn)
{
	if (field == nullptr)
		throw std::logic_error("Field is null");

	if (winningCombinationLength <= 0)
		throw std::logic_error("Winning combination length is incorrect");

	m_field = std::move(field);
	m_winningCombinationLength = winningCombinationLength;
	m_isFirstPlayerTurn = isFirstPlayerTurn;
}

void GameWorld::Reset(Figure::Type firstPlayerFigureType) noexcept
{
	m_firstPlayerFigureType = firstPlayerFigureType;
	Reset();
}

void GameWorld::Reset() noexcept
{
	m_field->Clear();
	m_isFirstPlayerTurn = true;
}

void GameWorld::SwitchTurn() noexcept
{
	m_isFirstPlayerTurn = !m_isFirstPlayerTurn;
}
