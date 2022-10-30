#pragma once
#include "WinapiException.h"
#include "Vector2D.h"
#include "Box2D.h"

template <typename T>
concept SelectableGDIOBJ = std::same_as<HBRUSH, T> || std::same_as<HPEN, T> ||
	std::same_as<HFONT, T> || std::same_as<HBITMAP, T> || std::same_as<HRGN, T>;

template <typename T>
concept GDIOBJ = SelectableGDIOBJ<T> || std::same_as<HGDIOBJ, T> || std::same_as<HPALETTE, T>;

std::string GetErrorDescription(HRESULT errorCode) noexcept;
std::wstring GetApplicationDirectory(HMODULE application);

Vector2i CalculateWindowSize(Vector2i windowClientAreaSize, DWORD windowStyle, bool hasMenu);

Vector2i GetWindowSize(HWND window);
Vector2i GetWindowClientAreaSize(HWND window);
Vector2i GetWindowPosition(HWND window);
Box2i GetWindowBox(HWND window);

Vector2i GetScreenSize() noexcept;
Vector2i GetMousePosition();

HFONT CreateWinapiFont(int characterSize, UINT style, const std::wstring& fontName = L"");
