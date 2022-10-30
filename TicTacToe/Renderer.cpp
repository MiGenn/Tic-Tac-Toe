#include "Renderer.h"

Renderer::Renderer(GameWindow& gameWindow) noexcept :
	m_graphics(gameWindow.GetGraphics()), m_camera(gameWindow.GetCamera())
{

}
