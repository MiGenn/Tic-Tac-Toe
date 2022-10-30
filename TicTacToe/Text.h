#pragma once
#include <string>
#include "ColorRGB.h"

class Text final
{
public:
	enum Style : unsigned char
	{
		Normal = 0,
		Bold = 0x01,
		Italic = 0x02,
		Underline = 0x04,
		StrikeThrough = 0x08
	};

	std::wstring text;
	unsigned char style{ Style::Normal };
	ColorRGB color;

	Text() noexcept = default;
	Text(const std::wstring& text, ColorRGB color) noexcept;

	void SetFont(const std::wstring& fontName);
	void SetCharacterSize(int size);

	const std::wstring& GetFontName() const noexcept;
	int GetCharacterSize() const noexcept;

private:
	std::wstring m_fontName;
	int m_characterSize{ 30 };
};
