#include "CombinationChecker.h"
#include "GameWorld.h"
#include "NumericUtilities.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

std::vector<Vector2i> CombinationChecker::m_cellIndices;

bool CombinationChecker::IsThereWinningCombinationForFigure(Figure figure, const IField& field, int combinationLength)
{
	if (combinationLength <= 0)
		throw std::logic_error("Combination length is incorrect");

	for (auto i{ 0 }; i < m_axisCount; ++i)
	{
		m_cellIndices.clear();
		const auto axis{ (Axis)i };

		FillWithSameTypeFigureCellIndicesOnAxis(figure, axis, field.GetFigures(), m_cellIndices);
		SortCellIndicesOnSameAxisInAscendOrder(m_cellIndices, axis);
		const auto combinationLengthOnAxis{ GetNearestCellIndicesCountOnAxis(figure.cellIndex, axis, m_cellIndices) + 1 };
		if (combinationLengthOnAxis >= combinationLength)
			return true;
	}

	return false;
}

void CombinationChecker::FillWithSameTypeFigureCellIndicesOnAxis(Figure mainFigure, Axis axis,
	const std::vector<Figure>& fieldFigures, std::vector<Vector2i>& outputFigureCellIndices) noexcept
{
	for (const auto figure : fieldFigures)
		if (figure.figureType == mainFigure.figureType && AreOnSameAxis(axis, figure.cellIndex, mainFigure.cellIndex))
			outputFigureCellIndices.push_back(figure.cellIndex);
}

bool CombinationChecker::AreOnSameAxis(Axis axis, Vector2i firstCellIndex, Vector2i secondCellIndex)
{
	switch (axis)
	{
		case Axis::Horizontal:
			return AreOnSameHorizontalAxis(firstCellIndex, secondCellIndex);

		case Axis::Vertical:
			return AreOnSameVerticalAxis(firstCellIndex, secondCellIndex);

		case Axis::FirstDiagonal:
			return AreOnSameFirstDiagonalAxis(firstCellIndex, secondCellIndex);

		case Axis::SecondDiagonal:
			return AreOnSameSecondDiagonalAxis(firstCellIndex, secondCellIndex);

		default:
			throw std::logic_error("Switch enum Axis error");
	}
}

bool CombinationChecker::AreOnSameHorizontalAxis(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept
{
	return firstCellIndex.y == secondCellIndex.y;
}

bool CombinationChecker::AreOnSameVerticalAxis(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept
{
	return firstCellIndex.x == secondCellIndex.x;
}

bool CombinationChecker::AreOnSameFirstDiagonalAxis(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept
{
	auto difference{ firstCellIndex - secondCellIndex };
	return difference.x == difference.y;
}

bool CombinationChecker::AreOnSameSecondDiagonalAxis(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept
{
	auto difference{ firstCellIndex - secondCellIndex };
	return HaveDifferentSigns(difference.x, difference.y) && 
		std::abs(difference.x) == std::abs(difference.y) ||
		difference == Vector2i(0, 0);
}

void CombinationChecker::SortCellIndicesOnSameAxisInAscendOrder(std::vector<Vector2i>& cellIndices, Axis axis) noexcept
{
	std::sort(cellIndices.begin(), cellIndices.end(), GetSortPredicateForAxis(axis));
}

CombinationChecker::AscendPredicate CombinationChecker::GetSortPredicateForAxis(Axis axis) noexcept
{
	if (axis == Axis::Vertical)
		return AscendYPredicate;

	return AscendXPredicate;
}

bool CombinationChecker::AscendXPredicate(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept
{
	return firstCellIndex.x < secondCellIndex.x;
}

bool CombinationChecker::AscendYPredicate(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept
{
	return firstCellIndex.y < secondCellIndex.y;
}

int CombinationChecker::GetNearestCellIndicesCountOnAxis(Vector2i mainCellIndex, Axis axis, 
	const std::vector<Vector2i>& sortedCellIndices)
{
	auto cellIndexIndex{ FindCellIndexIndex(m_cellIndices, mainCellIndex) };
	if (cellIndexIndex < 0)
		throw std::logic_error("Figure isn't in field");

	auto differenceFunction{ GetDifferenceFunctionForAxis(axis) };
	auto count{ 0 };

	for (auto i{ cellIndexIndex + 1 }; i < sortedCellIndices.size(); ++i)
	{
		if (differenceFunction(sortedCellIndices[i], sortedCellIndices[i - 1]) == 1)
			++count;
		else
			break;
	}

	for (auto i{ cellIndexIndex - 1 }; i > -1; --i)
	{
		if (differenceFunction(sortedCellIndices[i], sortedCellIndices[i + 1]) == -1)
			++count;
		else
			break;
	}

	return count;
}

int CombinationChecker::FindCellIndexIndex(const std::vector<Vector2i>& cellIndices, Vector2i cellIndexToSearch) noexcept
{
	for (auto i{ 0 }; i < cellIndices.size(); ++i)
		if (cellIndices[i] == cellIndexToSearch)
			return i;

	return -1;
}

CombinationChecker::DifferenceFunction CombinationChecker::GetDifferenceFunctionForAxis(Axis axis) noexcept
{
	if (axis == Axis::Vertical)
		return DifferenceYFunction;

	return DifferenceXFunction;
}

int CombinationChecker::DifferenceXFunction(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept
{
	return firstCellIndex.x - secondCellIndex.x;
}

int CombinationChecker::DifferenceYFunction(Vector2i firstCellIndex, Vector2i secondCellIndex) noexcept
{
	return firstCellIndex.y - secondCellIndex.y;
}
