#pragma once
#include <bitset>

class Keyboard final
{
public:
	friend class GameWindow;

	bool IsKeyPressed(unsigned char keycode) const noexcept;
	bool IsKeyHeld(unsigned char keycode) const noexcept;
	bool IsKeyUnpressed(unsigned char keycode) const noexcept;

private:
	static constexpr size_t m_keysNumber{ 256ull };

	std::bitset<m_keysNumber> m_keyPressedStates;
	std::bitset<m_keysNumber> m_keyHeldStates;
	std::bitset<m_keysNumber> m_keyUnpressedStates;

	Keyboard() noexcept = default;
	Keyboard(const Keyboard&) = delete;

	Keyboard& operator=(const Keyboard&) = delete;

	void ResetState() noexcept;

	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyUnpressed(unsigned char keycode) noexcept;
};
