#include "BackgroundRenderer.h"

BackgroundRenderer::BackgroundRenderer(const TicTacToe& gameApplication,
	GameWindow& gameWindow) noexcept : Renderer(gameWindow),
	m_gameApplication(gameApplication)
{

}

void BackgroundRenderer::Render()
{
	m_graphics.Clear(m_gameApplication.backgroundColor);
}
