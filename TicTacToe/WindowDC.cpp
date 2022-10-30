#include "WindowDC.h"
#include <stdexcept>

WindowDC::WindowDC(HWND window) :
	m_window(window), m_windowDC(GetDC(window))
{
	if (window == nullptr)
		throw std::logic_error("Window is null");
}

WindowDC::~WindowDC() noexcept
{
	ReleaseDC(m_window, m_windowDC);
}

HDC WindowDC::Get() const noexcept
{
	return m_windowDC;
}
