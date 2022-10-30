#pragma once
#include <bitset>
#include "Vector2D.h"

class Mouse final
{
public:
	friend class GameWindow;

	enum class Button
	{
		Left,
		Right,
		Middle
	};

	bool IsButtonPressed(Button button) const noexcept;
	bool IsButtonHeld(Button button) const noexcept;
	bool IsButtonUnpressed(Button button) const noexcept;

	/// <summary>
	/// Позиція миші оновлюється лише при натисканні однієї з її кнопок
	/// </summary>
	Vector2i GetPosition() const noexcept;
	int GetScrollDelta() const noexcept;

private:
	static constexpr size_t m_buttonsNumber{ 3ull };

	std::bitset<m_buttonsNumber> m_buttonPressedStates;
	std::bitset<m_buttonsNumber> m_buttonHeldStates;
	std::bitset<m_buttonsNumber> m_buttonUnpressedStates;

	Vector2i m_mousePosition;
	int m_scrollDelta{ 0 };

	Mouse() noexcept = default;
	Mouse(const Mouse&) = delete;

	Mouse& operator=(const Mouse&) = delete;

	void ResetState() noexcept;

	void OnButtonPressed(Button button, Vector2i windowCursorPosition) noexcept;
	void OnButtonUnpressed(Button button, Vector2i windowCursorPosition) noexcept;
	void OnWheelScrolled(int scrollDelta) noexcept;
};
