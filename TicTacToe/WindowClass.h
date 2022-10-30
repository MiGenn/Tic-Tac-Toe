#pragma once
#include "Window.h"

class WindowClass
{
public:
	const wchar_t* GetName() const noexcept;

protected:
	WindowClass(const wchar_t* name);
	WindowClass(const WindowClass&) = delete;
	virtual ~WindowClass() noexcept;

	WindowClass& operator=(const WindowClass&) = delete;

	static LRESULT SetupMessageHandling(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:
	const wchar_t* const name;

	static LRESULT RouteMessage(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
