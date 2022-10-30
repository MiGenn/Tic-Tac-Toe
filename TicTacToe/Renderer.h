#pragma once
#include "GameWindow.h"

class Renderer
{
public:
	Renderer(GameWindow& gameWindow) noexcept;
	Renderer(const Renderer&) = delete;
	virtual ~Renderer() noexcept = default;

	Renderer& operator=(const Renderer&) = delete;

	virtual void Render() = 0;

protected:
	Graphics& m_graphics;
	const Camera& m_camera;
};
