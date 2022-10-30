#pragma once
#include "WinapiException.h"

class Window
{
public:
	friend class WindowClass;

	Window() noexcept = default;
	Window(const Window&) = delete;
	virtual ~Window() noexcept = default;

	Window& operator=(const Window&) = delete;

protected:
	HWND m_handle{ nullptr };

	virtual LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;
};
