#pragma once
#include <memory>
#include "IRenderableField.h"
#include "Figure.h"
#include "Box2D.h"

class GameWorld final
{
public:
	enum class Player
	{
		FirstPlayer,
		SecondPlayer
	};

	GameWorld() = delete;
	GameWorld(const GameWorld&) = delete;

	GameWorld& operator=(const GameWorld&) = delete;

	static bool TryPlaceCurrentPlayerFigure(Vector2i cellIndex) noexcept;

	static Figure::Type GetFirstPlayerFigureType() noexcept;
	static Figure::Type GetSecondPlayerFigureType() noexcept;

	static Player GetCurrentPlayer() noexcept;
	static Player GetNextPlayer() noexcept;
	static Figure::Type GetCurrentPlayerFigureType() noexcept;
	static Figure::Type GetNextPlayerFigureType() noexcept;

	static const IRenderableField& GetField() noexcept;
	static int GetWinningCombinationLength() noexcept;

	static void Reset(std::unique_ptr<IRenderableField>&& field, int winningCombinationLength, bool isFirstPlayerTurn = true);
	static void Reset(Figure::Type firstPlayerFigureType) noexcept;
	static void Reset() noexcept;

private:
	static std::unique_ptr<IRenderableField> m_field;
	static inline int m_winningCombinationLength{ 1 };
	static inline Figure::Type m_firstPlayerFigureType;
	static bool m_isFirstPlayerTurn;

	static void SwitchTurn() noexcept;
};
