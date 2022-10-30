#pragma once
#include "GameWindow.h"

class Script
{
public:
	Script(GameWindow& gameWindow) noexcept;
	Script(const Script&) = delete;
	virtual ~Script() noexcept = default;

	Script& operator=(const Script&) = delete;

	virtual void Execute() = 0;

protected:
	Camera& m_camera;
	Keyboard& m_keyboard;
	Mouse& m_mouse;
};
