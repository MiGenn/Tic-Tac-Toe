#include "Script.h"

Script::Script(GameWindow& gameWindow) noexcept :
	m_camera(gameWindow.GetCamera()), m_keyboard(gameWindow.keyboard), m_mouse(gameWindow.mouse)
{

}
