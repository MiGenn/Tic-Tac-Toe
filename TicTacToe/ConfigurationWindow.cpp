#include "ConfigurationWindow.h"
#include "Resource.h"
#include "TicTacToe.h"
#include "GameWorld.h"
#include "WindowDC.h"
#include "TypeUtilities.h"
#include "WinapiUtilities.h"
#include "WindowUtilities.h"
#include "commdlg.h"

ConfigurationWindow::ConfigurationWindow(TicTacToe& gameApplication, GameWindow& mainGameWindow) :
	m_gameApplication(gameApplication), m_mainGameWindow(mainGameWindow)
{
	static constexpr UINT style{ WS_CAPTION | WS_SYSMENU };
	const auto windowSize{ CalculateWindowSize(clientAreaSize, style, false) };
	const auto position{ m_mainGameWindow.GetCenterPosition() - windowSize / 2 };

	m_handle = CreateWindow(ConfigurationWindowClass::configurationWindowClass.GetName(), 
		title.c_str(), style,position.x, position.y, windowSize.x, windowSize.y, 
		nullptr, nullptr, GetModuleHandle(nullptr), reinterpret_cast<Window*>(this));

	if (m_handle == nullptr)
		throw WINAPI_LAST_EXCEPTION();
}

ConfigurationWindow::~ConfigurationWindow() noexcept
{
	DestroyWindow(m_handle);
	m_handle = nullptr;
}

void ConfigurationWindow::StartConfiguration()
{
	m_mainGameWindow.Deactivate();
	m_mainGameWindow.Show();

	m_firstPlayerFigureColor = m_gameApplication.firstPlayer.figureColor;
	m_secondPlayerFigureColor = m_gameApplication.secondPlayer.figureColor;

	InitEditPlayerNameControls();
	UpdateBackgroundColor();
	UpdatePosition();
	ShowWindow(m_handle, SW_SHOW);
}

void ConfigurationWindow::InitEditPlayerNameControls()
{
	if (!SetWindowText(m_editFirstPlayerNameControl, m_gameApplication.firstPlayer.name.c_str()))
		throw WINAPI_LAST_EXCEPTION();

	if (!SetWindowText(m_editSecondPlayerNameControl, m_gameApplication.secondPlayer.name.c_str()))
		throw WINAPI_LAST_EXCEPTION();
}

void ConfigurationWindow::UpdateBackgroundColor() noexcept
{
	auto brush{ CreateSolidBrush(ColorRGBToColorref(m_gameApplication.backgroundColor)) };
	SetClassLongPtr(m_handle, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
}

void ConfigurationWindow::UpdatePosition()
{
	const auto position{ m_mainGameWindow.GetCenterPosition() - GetWindowSize(m_handle) / 2 };
	if (!SetWindowPos(m_handle, (HWND)HWND_TOP, position.x, position.y, NULL, NULL, SWP_NOSIZE))
		throw WINAPI_LAST_EXCEPTION();
}

LRESULT ConfigurationWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			OnCreate();
			return 0;

		case WM_COMMAND:
			OnCommand((ControlID)LOWORD(wParam));
			return 0;

		case WM_CTLCOLOREDIT:
			OnSetEditControlBackgroundColor((HDC)wParam);
			return (LRESULT)GetStockObject(DC_BRUSH);

		case WM_DRAWITEM:
			OnDrawControl((ControlID)wParam, *(LPDRAWITEMSTRUCT)lParam);
			return 0;

		case WM_CLOSE:
			OnClose();
			return 0;
	}

	return DefWindowProc(m_handle, message, wParam, lParam);
}

void ConfigurationWindow::OnCreate() noexcept
{
	m_font = CreateWinapiFont(30, Text::Style::Normal, L"UNISPACE");

	CreateEditFirstPlayerNameControl();
	CreateEditSecondPlayerNameControl();
	CreateSetFirstPlayerColorButton();
	CreateSetSecondPlayerColorButton();
	CreateStartGameButton();
}

void ConfigurationWindow::CreateEditFirstPlayerNameControl()
{
	static constexpr Vector2i controlSize{ 150, 32 };
	static constexpr Vector2i controlPosition{ 10, 10 };

	m_editFirstPlayerNameControl = CreateWindow(L"EDIT", nullptr, ES_CENTER | WS_CHILD | WS_VISIBLE,
		controlPosition.x, controlPosition.y, controlSize.x, controlSize.y, m_handle,
		(HMENU)ControlID::EditFirstPlayerName, GetModuleHandle(nullptr), nullptr);

	if (m_editFirstPlayerNameControl == nullptr)
		throw std::logic_error("Cannot create edit first player name control");

	SendMessage(m_editFirstPlayerNameControl, WM_SETFONT, (WPARAM)m_font.Get(), (LPARAM)true);
}

void ConfigurationWindow::CreateEditSecondPlayerNameControl()
{
	static constexpr Vector2i controlSize{ 150, 32 };
	static constexpr Vector2i controlPosition{ clientAreaSize.x - controlSize.x - 10, 10 };

	m_editSecondPlayerNameControl = CreateWindow(L"EDIT", nullptr, ES_CENTER | WS_CHILD | WS_VISIBLE,
		controlPosition.x, controlPosition.y, controlSize.x, controlSize.y, m_handle,
		(HMENU)ControlID::EditSecondPlayerName, GetModuleHandle(nullptr), nullptr);

	if (m_editSecondPlayerNameControl == nullptr)
		throw std::logic_error("Cannot create edit second player name control");

	SendMessage(m_editSecondPlayerNameControl, WM_SETFONT, (WPARAM)m_font.Get(), (LPARAM)true);
}

void ConfigurationWindow::CreateSetFirstPlayerColorButton()
{
	static constexpr Vector2i buttonSize{ 100, 100 };
	static constexpr Vector2i buttonPosition{ clientAreaSize.x / 16 + 14, (clientAreaSize.y - buttonSize.y) / 2 - 10 };

	m_setFirstPlayerColorButton = CreateWindow(L"BUTTON", nullptr, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE,
		buttonPosition.x, buttonPosition.y, buttonSize.x, buttonSize.y, m_handle,
		(HMENU)ControlID::SetFirstPlayerColorButton, GetModuleHandle(nullptr), nullptr);

	if (m_setFirstPlayerColorButton == nullptr)
		throw std::logic_error("Cannot create first player color button");

	SubscribeOnMouseLeaveOrEnterEvent(m_setFirstPlayerColorButton, [this](bool isMouseEnteredWindow)
		{
			m_isSetFirstPlayerColorButtonHovered = isMouseEnteredWindow;
			InvalidateRect(m_setFirstPlayerColorButton, nullptr, false);
		});
}

void ConfigurationWindow::CreateSetSecondPlayerColorButton()
{
	static constexpr Vector2i buttonSize{ 100, 100 };
	static constexpr Vector2i buttonPosition{ clientAreaSize.x - clientAreaSize.x / 16 - buttonSize.x - 14,
		(clientAreaSize.y - buttonSize.y) / 2 - 10 };

	m_setSecondPlayerColorButton = CreateWindow(L"BUTTON", nullptr, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE,
		buttonPosition.x, buttonPosition.y, buttonSize.x, buttonSize.y, m_handle,
		(HMENU)ControlID::SetSecondPlayerColorButton, GetModuleHandle(nullptr), nullptr);

	if (m_setSecondPlayerColorButton == nullptr)
		throw std::logic_error("Cannot create second player color button");

	SubscribeOnMouseLeaveOrEnterEvent(m_setSecondPlayerColorButton, [this](bool isMouseEnteredWindow)
		{
			m_isSetSecondPlayerColorButtonHovered = isMouseEnteredWindow;
			InvalidateRect(m_setSecondPlayerColorButton, nullptr, false);
		});
}

void ConfigurationWindow::CreateStartGameButton()
{
	static constexpr Vector2i buttonSize{ clientAreaSize.x - 20, 50 };
	static constexpr Vector2i buttonPosition{ 10, clientAreaSize.y - buttonSize.y - 10 };

	m_startGameButton = CreateWindow(L"BUTTON", L"Start", BS_OWNERDRAW | WS_CHILD | WS_VISIBLE,
		buttonPosition.x, buttonPosition.y, buttonSize.x, buttonSize.y, m_handle,
		(HMENU)ControlID::StartGameButton, GetModuleHandle(nullptr), nullptr);

	if (m_startGameButton == nullptr)
		throw std::logic_error("Cannot create start game button");

	SubscribeOnMouseLeaveOrEnterEvent(m_startGameButton, [this](bool isMouseEnteredWindow)
		{
			m_isStartGameButtonHovered = isMouseEnteredWindow;
			InvalidateRect(m_startGameButton, nullptr, false);
		});
}

void ConfigurationWindow::OnCommand(ControlID controlID)
{
	switch (controlID)
	{
	case ControlID::SetFirstPlayerColorButton:
		OnSetFirstPlayerColorButtonPressed();
		break;

	case ControlID::SetSecondPlayerColorButton:
		OnSetSecondPlayerColorButtonPressed();
		break;

	case ControlID::StartGameButton:
		OnStartGameButtonPressed();
		break;
	}
}

void ConfigurationWindow::OnSetFirstPlayerColorButtonPressed() noexcept
{
	COLORREF acrCustClr[16];

	CHOOSECOLOR cc{ sizeof(cc) };
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.hwndOwner = m_handle;
	cc.rgbResult = ColorRGBToColorref(m_firstPlayerFigureColor);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc))
	{
		m_firstPlayerFigureColor = ColorrefToColorRGB(cc.rgbResult);
		InvalidateRect(m_setFirstPlayerColorButton, nullptr, false);
	}
}

void ConfigurationWindow::OnSetSecondPlayerColorButtonPressed() noexcept
{
	COLORREF acrCustClr[16];

	CHOOSECOLOR cc{ sizeof(cc) };
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.hwndOwner = m_handle;
	cc.rgbResult = ColorRGBToColorref(m_secondPlayerFigureColor);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc))
	{
		m_secondPlayerFigureColor = ColorrefToColorRGB(cc.rgbResult);
		InvalidateRect(m_setSecondPlayerColorButton, nullptr, false);
	}
}

void ConfigurationWindow::OnStartGameButtonPressed() noexcept
{
	const auto [firstPlayerName, secondPlayerName] { ParseEditControls() };
	if (firstPlayerName.size() != 0 && secondPlayerName.size() != 0)
	{
		m_gameApplication.firstPlayer = { firstPlayerName, m_firstPlayerFigureColor };
		m_gameApplication.secondPlayer = { secondPlayerName, m_secondPlayerFigureColor };

		GameWorld::Reset();
		m_mainGameWindow.Rerender();
		ShowWindow(m_handle, SW_HIDE);
		m_mainGameWindow.Activate();
		m_mainGameWindow.Show();
	}
	else
	{
		MessageBox(m_handle, L"Error", L"Name field is empty", MB_ICONERROR);
	}
}

std::pair<std::wstring, std::wstring> ConfigurationWindow::ParseEditControls() const noexcept
{
	static constexpr size_t playerNameBufferSize{ 16ull };

	wchar_t firstPlayerNameBuffer[playerNameBufferSize];
	wchar_t secondPlayerNameBuffer[playerNameBufferSize];

	GetWindowText(m_editFirstPlayerNameControl, firstPlayerNameBuffer, playerNameBufferSize);
	GetWindowText(m_editSecondPlayerNameControl, secondPlayerNameBuffer, playerNameBufferSize);

	return { firstPlayerNameBuffer, secondPlayerNameBuffer };
}

void ConfigurationWindow::OnSetEditControlBackgroundColor(HDC controlDC)
{
	SetBkMode(controlDC, TRANSPARENT);
	SetDCBrushColor(controlDC, ColorRGBToColorref(m_gameApplication.backgroundColor));
}

void ConfigurationWindow::OnDrawControl(ControlID controlID, DRAWITEMSTRUCT& controlDrawInfo)
{
	switch (controlID)
	{
		case ControlID::SetFirstPlayerColorButton:
			OnRenderSetFirstPlayerColorButton(controlDrawInfo);
			break;

		case ControlID::SetSecondPlayerColorButton:
			OnRenderSetSecondPlayerColorButton(controlDrawInfo);
			break;

		case ControlID::StartGameButton:
			OnRenderStartGameButton(controlDrawInfo);
			break;
	}
}

void ConfigurationWindow::OnRenderSetFirstPlayerColorButton(DRAWITEMSTRUCT& drawInfo) noexcept
{
	auto isClicked{ drawInfo.itemState & ODS_SELECTED };
	if (!isClicked && m_isSetFirstPlayerColorButtonHovered)
		drawInfo.itemState = ODS_HOTLIGHT;

	RenderSetFirstPlayerColorButton(drawInfo);
}

void ConfigurationWindow::OnRenderSetSecondPlayerColorButton(DRAWITEMSTRUCT& drawInfo) noexcept
{
	auto isClicked{ drawInfo.itemState & ODS_SELECTED };
	if (!isClicked && m_isSetSecondPlayerColorButtonHovered)
		drawInfo.itemState = ODS_HOTLIGHT;

	RenderSetSecondPlayerColorButton(drawInfo);
}

void ConfigurationWindow::OnRenderStartGameButton(DRAWITEMSTRUCT& drawInfo) noexcept
{
	auto isClicked{ drawInfo.itemState & ODS_SELECTED };
	if (!isClicked && m_isStartGameButtonHovered)
		drawInfo.itemState = ODS_HOTLIGHT;

	RenderStartGameButton(drawInfo);
}

void ConfigurationWindow::RenderSetFirstPlayerColorButton(const DRAWITEMSTRUCT& drawInfo) noexcept
{
	WindowDC buttonDC(m_setFirstPlayerColorButton);
	RenderSetColorButton(buttonDC.Get(), drawInfo, m_firstPlayerFigureColor);
}

void ConfigurationWindow::RenderSetSecondPlayerColorButton(const DRAWITEMSTRUCT& drawInfo) noexcept
{
	WindowDC buttonDC(m_setSecondPlayerColorButton);
	RenderSetColorButton(buttonDC.Get(), drawInfo, m_secondPlayerFigureColor);
}

void ConfigurationWindow::RenderSetColorButton(HDC buttonDC, const DRAWITEMSTRUCT& drawInfo, ColorRGB buttonColor) noexcept
{
	static constexpr auto penStyle{ PS_SOLID };
	static constexpr auto penThickness{ 1 };
	static constexpr auto penColor{ RGB(0, 0, 0) };
	static constexpr ColorRGB selectionColor{ 0, 0, 0 };

	if (drawInfo.itemState & ODS_HOTLIGHT)
		buttonColor = buttonColor.Blend(selectionColor, 20);
	else if (drawInfo.itemState & ODS_SELECTED)
		buttonColor = buttonColor.Blend(selectionColor, 36);

	UniqueHGDIOBJ pen(CreatePen(penStyle, penThickness, penColor));
	UniqueHGDIOBJ brush(CreateSolidBrush(ColorRGBToColorref(buttonColor)));
	SelectionWrapper penSelection(buttonDC, pen.Get());
	SelectionWrapper brushSelection(buttonDC, brush.Get());

	Rectangle(buttonDC, drawInfo.rcItem.left, drawInfo.rcItem.top, drawInfo.rcItem.right, drawInfo.rcItem.bottom);
}

void ConfigurationWindow::RenderStartGameButton(const DRAWITEMSTRUCT& drawInfo) noexcept
{
	static constexpr auto penStyle{ PS_SOLID };
	static constexpr auto penThickness{ 1 };
	static constexpr auto penColor{ RGB(0, 0, 0) };
	static constexpr ColorRGB selectionColor{ 0, 0, 0 };
	static constexpr auto textFormat{ (UINT)TextFormat::AlignmentX::Center | (UINT)TextFormat::AlignmentY::Center };

	ColorRGB backgroundColor(255, 255, 255);
	if (drawInfo.itemState & ODS_HOTLIGHT)
		backgroundColor = backgroundColor.Blend(selectionColor, 20);
	else if (drawInfo.itemState & ODS_SELECTED)
		backgroundColor = backgroundColor.Blend(selectionColor, 36);

	WindowDC buttonDC(m_startGameButton);

	UniqueHGDIOBJ brush(CreateSolidBrush(ColorRGBToColorref(m_gameApplication.backgroundColor)));
	SelectionWrapper penSelection(buttonDC.Get(), (HPEN)GetStockObject(NULL_PEN));
	SelectionWrapper brushSelection(buttonDC.Get(), brush.Get());
	Rectangle(buttonDC.Get(), drawInfo.rcItem.left, drawInfo.rcItem.top, drawInfo.rcItem.right + 1, drawInfo.rcItem.bottom + 1);

	UniqueHGDIOBJ pen(CreatePen(penStyle, penThickness, penColor));
	brush.Reset(CreateSolidBrush(ColorRGBToColorref(backgroundColor)));
	penSelection.Select(buttonDC.Get(), pen.Get());
	brushSelection.Select(buttonDC.Get(), brush.Get());
	RoundRect(buttonDC.Get(), drawInfo.rcItem.left, drawInfo.rcItem.top, drawInfo.rcItem.right, drawInfo.rcItem.bottom, 46, 46);

	SetBkMode(buttonDC.Get(), TRANSPARENT);
	RECT buttonRect(drawInfo.rcItem);
	SelectionWrapper fontSelection(buttonDC.Get(), m_font.Get());
	DrawText(buttonDC.Get(), L"Start", -1, &buttonRect, textFormat);
}

void ConfigurationWindow::OnClose() noexcept
{
	PostQuitMessage(0);
}

const ConfigurationWindow::ConfigurationWindowClass ConfigurationWindow::ConfigurationWindowClass::configurationWindowClass;

ConfigurationWindow::ConfigurationWindowClass::ConfigurationWindowClass() : WindowClass(L"ConfigurationWindowClass")
{
	WNDCLASSEX windowClass{ sizeof(windowClass) };
	windowClass.lpszClassName = GetName();
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.lpfnWndProc = SetupMessageHandling;
	windowClass.hIcon = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_ICON));
	windowClass.hIconSm = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_ICON));
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

	if (!RegisterClassEx(&windowClass))
		throw WINAPI_LAST_EXCEPTION();
}
