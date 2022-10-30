#pragma once

class ColorRGB final
{
public:
	unsigned char red{ 0 };
	unsigned char green{ 0 };
	unsigned char blue{ 0 };

	constexpr ColorRGB() noexcept = default;

	constexpr ColorRGB(unsigned char red, unsigned char green, unsigned char blue) noexcept :
		red(red), green(green), blue(blue)
	{

	}

	constexpr ColorRGB Blend(ColorRGB transparentColor, unsigned char transparency) const noexcept
	{
		auto transparencyInt{ (unsigned int)transparency };

		auto blendedRed{ (red * (255u - transparencyInt) + transparentColor.red * transparencyInt) / 255u };
		auto blendedGree{ (green * (255u - transparencyInt) + transparentColor.green * transparencyInt) / 255u };
		auto blendedBlue{ (blue * (255u - transparencyInt) + transparentColor.blue * transparencyInt) / 255u };

		return { (unsigned char)blendedRed, (unsigned char)blendedGree, (unsigned char)blendedBlue };
	}
};
