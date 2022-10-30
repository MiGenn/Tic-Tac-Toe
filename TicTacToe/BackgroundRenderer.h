#pragma once
#include "Renderer.h" 
#include "TicTacToe.h"

class BackgroundRenderer final : public Renderer
{
public:
	BackgroundRenderer(const TicTacToe& gameApplication, GameWindow& gameWindow) noexcept;
	BackgroundRenderer(const BackgroundRenderer&) = delete;

	BackgroundRenderer& operator=(const BackgroundRenderer&) = delete;

	virtual void Render() override;

private:
	const TicTacToe& m_gameApplication;
};
