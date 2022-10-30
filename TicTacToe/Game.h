#pragma once
#include "GameWindow.h"
#include "WinapiUtilities.h"

class Game
{
public:
	const std::wstring directory{ GetApplicationDirectory(nullptr) };

	Game(const std::wstring& windowTitle, Vector2i windowSize, bool canBeResizedByDraggingBorders);
	Game(const Game&) = delete;
	virtual ~Game() noexcept = default;

	Game& operator=(const Game&) = delete;

	int Run();

protected:
	GameWindow m_mainWindow;

private:
	int DispatchMessages();
};
