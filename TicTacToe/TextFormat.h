#pragma once
#include "WinapiException.h"

class TextFormat final
{
public:
	enum class AlignmentX : unsigned int
	{
		Left = DT_LEFT,
		Right = DT_RIGHT,
		Center = DT_CENTER
	};

	enum class AlignmentY : unsigned int
	{
		Top = DT_TOP,
		Bottom = DT_BOTTOM | DT_SINGLELINE,
		Center = DT_VCENTER | DT_SINGLELINE
	};

	enum class Overflow : unsigned int
	{
		Clip = 0,
		Ellipsis = DT_WORD_ELLIPSIS,
		WordBreak = DT_WORDBREAK
	};

	AlignmentX alignmentX{ AlignmentX::Left };
	AlignmentY alignmentY{ AlignmentY::Top };
	Overflow overflow{ Overflow::Ellipsis };

	constexpr TextFormat() noexcept = default;
	constexpr TextFormat(AlignmentX alignmentX, Overflow overflow) noexcept :
		alignmentX(alignmentX), overflow(overflow)
	{

	}

	constexpr TextFormat(AlignmentX alignmentX, AlignmentY alignmentY, Overflow overflow) noexcept :
		alignmentX(alignmentX), alignmentY(alignmentY), overflow(overflow)
	{

	}
};
