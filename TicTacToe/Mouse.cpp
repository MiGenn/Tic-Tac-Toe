#include "Mouse.h"
#include "WinapiException.h"

bool Mouse::IsButtonPressed(Button button) const noexcept
{
	return m_buttonPressedStates[(int)button];
}

bool Mouse::IsButtonHeld(Button button) const noexcept
{
	return m_buttonHeldStates[(int)button];
}

bool Mouse::IsButtonUnpressed(Button button) const noexcept
{
	return m_buttonUnpressedStates[(int)button];
}

Vector2i Mouse::GetPosition() const noexcept
{
	return m_mousePosition;
}

int Mouse::GetScrollDelta() const noexcept
{
	return m_scrollDelta;
}

void Mouse::ResetState() noexcept
{
	m_buttonPressedStates.reset();
	m_buttonHeldStates.reset();
	m_buttonUnpressedStates.reset();
	m_scrollDelta = 0;
}

void Mouse::OnButtonPressed(Button button, Vector2i windowCursorPosition) noexcept
{
	const auto buttonCode{ (int)button };
	m_buttonPressedStates[buttonCode] = true;
	m_buttonHeldStates[buttonCode] = true;
	m_mousePosition = windowCursorPosition;
}

void Mouse::OnButtonUnpressed(Button button, Vector2i windowCursorPosition) noexcept
{
	const auto buttonCode{ (int)button };
	m_buttonUnpressedStates[buttonCode] = true;
	m_buttonHeldStates[buttonCode] = false;
	m_mousePosition = windowCursorPosition;
}

void Mouse::OnWheelScrolled(int scrollDelta) noexcept
{
	m_scrollDelta = scrollDelta;
}
