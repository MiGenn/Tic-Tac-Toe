#include "Keyboard.h"

bool Keyboard::IsKeyPressed(unsigned char keycode) const noexcept
{
    return m_keyPressedStates[keycode];
}

bool Keyboard::IsKeyHeld(unsigned char keycode) const noexcept
{
    return m_keyHeldStates[keycode];
}

bool Keyboard::IsKeyUnpressed(unsigned char keycode) const noexcept
{
    return m_keyUnpressedStates[keycode];
}

void Keyboard::ResetState() noexcept
{
    m_keyPressedStates.reset();
    m_keyHeldStates.reset();
    m_keyUnpressedStates.reset();
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    m_keyPressedStates[keycode] = true;
    m_keyHeldStates[keycode] = true;
}

void Keyboard::OnKeyUnpressed(unsigned char keycode) noexcept
{
    m_keyUnpressedStates[keycode] = true;
    m_keyHeldStates[keycode] = false;
}
