#include "TicTacToe.h"
#include "Resource.h"
#include "GameWorld.h"
#include "InfiniteField.h"
#include "FieldMoverScript.h"
#include "CameraZoomerScript.h"
#include "FigurePlacerScript.h"
#include "BackgroundRenderer.h"
#include "TurnInfoRenderer.h"
#include "FiguresRenderer.h"
#include "FieldRenderer.h"

TicTacToe::TicTacToe() : Game(L"Tic-Tac-Toe", { 1280, 720 }, true),
	m_configurationWindow(*this, m_mainWindow)
{
	Configure();
	LoadScripts();

	m_mainWindow.Show();
	m_mainWindow.Rerender();
	m_configurationWindow.StartConfiguration();
}

const PlayerNonGameData& TicTacToe::GetCurrentPlayer() const noexcept
{
	if (GameWorld::GetCurrentPlayer() == GameWorld::Player::FirstPlayer)
		return firstPlayer;

	return secondPlayer;
}

const PlayerNonGameData& TicTacToe::GetNextPlayer() const noexcept
{
	if (GameWorld::GetNextPlayer() == GameWorld::Player::FirstPlayer)
		return firstPlayer;

	return secondPlayer;
}

void TicTacToe::Configure()
{
	m_mainWindow.SetMinSize({ 330, 250 });
	m_mainWindow.SetIcon(IDI_ICON);
	m_mainWindow.GetCamera().SetViewedWorldAreaSize({12.f, 8.f});
	GameWorld::Reset(std::make_unique<InfiniteField>(), 5);
}

void TicTacToe::LoadScripts()
{
	m_mainWindow.scripts.emplace_back(new FieldMoverScript(m_mainWindow));
	m_mainWindow.scripts.emplace_back(new CameraZoomerScript(m_mainWindow));
	m_mainWindow.scripts.emplace_back(new FigurePlacerScript(*this, m_configurationWindow, m_mainWindow));

	m_mainWindow.renderers.emplace_back(new BackgroundRenderer(*this, m_mainWindow));
	m_mainWindow.renderers.emplace_back(new FieldRenderer(*this, m_mainWindow));
	m_mainWindow.renderers.emplace_back(new FiguresRenderer(*this, m_mainWindow));
	m_mainWindow.renderers.emplace_back(new TurnInfoRenderer(*this, m_mainWindow));
}
