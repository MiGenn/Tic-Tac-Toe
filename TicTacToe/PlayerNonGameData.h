#pragma once
#include "WinapiException.h"
#include "ColorRGB.h"

class PlayerNonGameData final
{
public:
	std::wstring name;
	ColorRGB figureColor;

	PlayerNonGameData(const std::wstring& name, ColorRGB figureColor) noexcept;
};
