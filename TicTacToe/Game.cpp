#include "Game.h"

Game::Game(const std::wstring& windowTitle, Vector2i windowSize, bool canBeResizedByDraggingBorders) :
    m_mainWindow(windowTitle, windowSize, canBeResizedByDraggingBorders)
{
    
}

int Game::Run()
{
    return DispatchMessages();
}

int Game::DispatchMessages()
{
    MSG messageDescription{};
    while (GetMessage(&messageDescription, nullptr, NULL, NULL))
    {
        TranslateMessage(&messageDescription);
        DispatchMessage(&messageDescription);
    }

    return static_cast<int>(messageDescription.wParam);
}
