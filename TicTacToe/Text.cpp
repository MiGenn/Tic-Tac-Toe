#include "Text.h"
#include <stdexcept>

Text::Text(const std::wstring& text, ColorRGB color) noexcept :
    text(text), color(color)
{

}

void Text::SetFont(const std::wstring& fontName)
{
    m_fontName = fontName;
}

void Text::SetCharacterSize(int characterSize)
{
    if (characterSize <= 0)
        throw std::logic_error("Character size is incorrect");

    m_characterSize = characterSize;
}

const std::wstring& Text::GetFontName() const noexcept
{
    return m_fontName;
}

int Text::GetCharacterSize() const noexcept
{
    return m_characterSize;
}
