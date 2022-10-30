#include "WindowClass.h"
#include <stdexcept>

const wchar_t* WindowClass::GetName() const noexcept
{
	return name;
}

WindowClass::WindowClass(const wchar_t* name) :
	name(name)
{
	if (name == nullptr || wcslen(name) == 0)
		throw std::logic_error("Name isn't specified");
}

WindowClass::~WindowClass() noexcept
{
	UnregisterClass(name, GetModuleHandle(nullptr));
}

LRESULT WindowClass::SetupMessageHandling(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE)
	{
		const auto createStruct{ reinterpret_cast<CREATESTRUCT*>(lParam) };
		auto const window{ reinterpret_cast<Window*>(createStruct->lpCreateParams) };
		
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(RouteMessage));

		return RouteMessage(handle, message, wParam, lParam);
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

LRESULT WindowClass::RouteMessage(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto const window{ reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA)) };
	window->m_handle = handle;

	return window->HandleMessage(message, wParam, lParam);
}
