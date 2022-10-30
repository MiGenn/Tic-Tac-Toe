#pragma once
#include "WinapiException.h"

class WindowDC final
{
public:
	WindowDC(HWND window);
	WindowDC(const WindowDC&) = delete;
	~WindowDC() noexcept;

	WindowDC& operator=(const WindowDC&) = delete;

	HDC Get() const noexcept;

private:
	const HWND m_window;
	const HDC m_windowDC;
};
