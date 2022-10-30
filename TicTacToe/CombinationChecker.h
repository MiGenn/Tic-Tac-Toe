#pragma once
#include <vector>
#include "IField.h"

class CombinationChecker final
{
public:
	CombinationChecker() = delete;
	CombinationChecker(const CombinationChecker&) = delete;

	CombinationChecker& operator=(const CombinationChecker&) = delete;

	static bool IsThereWinningCombinationForFigure(Figure figure, const IField& field, int combinationLength);

private:
	using AscendPredicate = bool(*)(Vector2i, Vector2i);
	using DifferenceFunction = int(*)(Vector2i, Vector2i);

	enum class Axis
	{
		Horizontal,
		Vertical,
		FirstDiagonal,
		SecondDiagonal
	};

	static constexpr int m_axisCount{ 4 };

	static std::vector<Vector2i> m_cellIndices;

	static void FillWithSameTypeFigureCellIndicesOnAxis(Figure mainFigure, Axis axis,
		const std::vector<Figure>& fieldFigures, std::vector<Vector2i>& outputFigureCellIndices) noexcept;
	static bool AreOnSameAxis(Axis axis, Vector2i firstCellIndex, Vector2i secondCellIndex);
	static bool AreOnSameHorizontalAxis(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept;
	static bool AreOnSameVerticalAxis(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept;
	static bool AreOnSameFirstDiagonalAxis(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept;
	static bool AreOnSameSecondDiagonalAxis(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept;

	static void SortCellIndicesOnSameAxisInAscendOrder(std::vector<Vector2i>& cellIndices, Axis axis) noexcept;
	static AscendPredicate GetSortPredicateForAxis(Axis axis) noexcept;
	static bool AscendXPredicate(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept;
	static bool AscendYPredicate(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept;

	static int GetNearestCellIndicesCountOnAxis(Vector2i mainCellIndex, Axis axis, const std::vector<Vector2i>& sortedCellIndices);
	static int FindCellIndexIndex(const std::vector<Vector2i>& cellIndices, Vector2i cellIndexToSearch) noexcept;
	static DifferenceFunction GetDifferenceFunctionForAxis(Axis axis) noexcept;
	static int DifferenceXFunction(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept;
	static int DifferenceYFunction(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept;
};
