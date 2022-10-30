#pragma once
#include "WindowClass.h"
#include "Window.h"
#include "GameWindow.h"
#include "UniqueHGDIOBJ.h"

class TicTacToe;

class ConfigurationWindow final : public Window
{
public:
	static inline const std::wstring title{ L"Settings" };
	static inline constexpr Vector2i clientAreaSize{ 340, 220 };

	ConfigurationWindow(TicTacToe& gameApplication, GameWindow& mainGameWindow);
	ConfigurationWindow(const ConfigurationWindow&) = delete;
	virtual ~ConfigurationWindow() noexcept;

	ConfigurationWindow& operator=(const ConfigurationWindow&) = delete;

	void StartConfiguration();

private:
	enum class ControlID
	{
		EditFirstPlayerName,
		EditSecondPlayerName,

		SetFirstPlayerColorButton,
		SetSecondPlayerColorButton,

		StartGameButton
	};

	UniqueHGDIOBJ<HFONT> m_font;

	HWND m_editFirstPlayerNameControl;
	HWND m_editSecondPlayerNameControl;
	HWND m_setFirstPlayerColorButton;
	HWND m_setSecondPlayerColorButton;
	HWND m_startGameButton;

	bool m_isSetFirstPlayerColorButtonHovered{ false };
	bool m_isSetSecondPlayerColorButtonHovered{ false };
	bool m_isStartGameButtonHovered{ false };

	TicTacToe& m_gameApplication;
	GameWindow& m_mainGameWindow;

	ColorRGB m_firstPlayerFigureColor;
	ColorRGB m_secondPlayerFigureColor;

	void InitEditPlayerNameControls();
	void UpdateBackgroundColor() noexcept;
	void UpdatePosition();

	LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

	void OnCreate() noexcept;
	void CreateEditFirstPlayerNameControl();
	void CreateEditSecondPlayerNameControl();
	void CreateSetFirstPlayerColorButton();
	void CreateSetSecondPlayerColorButton();
	void CreateStartGameButton();

	void OnCommand(ControlID controlID);
	void OnSetFirstPlayerColorButtonPressed() noexcept;
	void OnSetSecondPlayerColorButtonPressed() noexcept;
	void OnStartGameButtonPressed() noexcept;

	std::pair<std::wstring, std::wstring> ParseEditControls() const noexcept;

	void OnDrawControl(ControlID controlID, DRAWITEMSTRUCT& controlDrawInfo);
	void OnSetEditControlBackgroundColor(HDC controlDC);
	void OnRenderSetFirstPlayerColorButton(DRAWITEMSTRUCT& drawInfo) noexcept;
	void OnRenderSetSecondPlayerColorButton(DRAWITEMSTRUCT& drawInfo) noexcept;
	void OnRenderStartGameButton(DRAWITEMSTRUCT& drawInfo) noexcept;

	void RenderSetFirstPlayerColorButton(const DRAWITEMSTRUCT& drawInfo) noexcept;
	void RenderSetSecondPlayerColorButton(const DRAWITEMSTRUCT& drawInfo) noexcept;
	void RenderSetColorButton(HDC buttonDC, const DRAWITEMSTRUCT& drawInfo, ColorRGB buttonColor) noexcept;
	void RenderStartGameButton(const DRAWITEMSTRUCT& drawInfo) noexcept;

	void OnClose() noexcept;

	class ConfigurationWindowClass final : public WindowClass
	{
	public:
		static const ConfigurationWindowClass configurationWindowClass;

	private:
		ConfigurationWindowClass();
		ConfigurationWindowClass(const ConfigurationWindowClass&) = delete;
		ConfigurationWindowClass& operator=(const ConfigurationWindowClass&) = delete;
	};
};
