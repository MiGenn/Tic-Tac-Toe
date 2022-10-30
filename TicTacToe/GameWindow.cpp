#include "GameWindow.h"
#include "Script.h"
#include "Renderer.h"
#include "Resource.h"
#include "TypeUtilities.h"
#include "WinapiUtilities.h"

GameWindow::GameWindow(const std::wstring& title, Vector2i size, bool canBeResizedByUser) :
	m_clientAreaSize(size), m_clientAreaSizeIsChanged(m_clientAreaSizeIsChangedRaiser)
{
	m_windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	if (canBeResizedByUser)
		m_windowStyle |= WS_SIZEBOX | WS_MAXIMIZEBOX;

	const auto windowSize{ CalculateWindowSize(m_clientAreaSize, m_windowStyle, false) };
	m_windowBordersSize = windowSize - m_clientAreaSize;
	m_position = (GetScreenSize() - windowSize) / 2; // ÷ентруЇ в≥кно

	m_handle = CreateWindow(GameWindowClass::gameWindowClass.GetName(), title.c_str(), 
		m_windowStyle, m_position.x, m_position.y, windowSize.x, windowSize.y, 
		nullptr, nullptr, GetModuleHandle(nullptr), reinterpret_cast<Window*>(this));

	if (m_handle == nullptr)
		throw WINAPI_LAST_EXCEPTION();

	m_graphics.reset(new Graphics(m_handle, m_clientAreaSizeIsChanged));
	m_camera.reset(new Camera(m_clientAreaSize, m_clientAreaSizeIsChanged));
}

GameWindow::~GameWindow() noexcept
{
	DestroyWindow(m_handle);
	m_handle = nullptr;
}

void GameWindow::SetPosition(Vector2i position)
{
	m_position = position;
	if (!SetWindowPos(m_handle, nullptr, position.x, position.y, NULL, NULL, SWP_NOZORDER | SWP_NOSIZE))
		throw WINAPI_LAST_EXCEPTION();
}

void GameWindow::SetCenterPosition(Vector2i centerPosition)
{
	SetPosition(centerPosition - GetWindowSize() / 2);
}

void GameWindow::Resize(Vector2i size)
{
	if (size.x < m_minClientAreaSize.x)
		size.x = m_minClientAreaSize.x;

	if (size.y < m_minClientAreaSize.y)
		size.y = m_minClientAreaSize.y;

	const auto centerPosition{ GetCenterPosition() };

	m_clientAreaSize = size;

	const auto newWindowSize{ GetWindowSize() };
	m_position = centerPosition - newWindowSize / 2;

	if (!SetWindowPos(m_handle, nullptr, m_position.x, m_position.y, newWindowSize.x, newWindowSize.y, SWP_NOZORDER))
		throw WINAPI_LAST_EXCEPTION();

	m_clientAreaSizeIsChangedRaiser.Raise(m_clientAreaSize);
	RunRenderers();
}

void GameWindow::SetMinSize(Vector2i minSize)
{
	if (minSize.x <= 0 || minSize.y <= 0)
		throw std::logic_error("Min size is incorrect");

	if (minSize.x < m_minPossibleClientAreaWidth)
		minSize.x = m_minPossibleClientAreaWidth;

	m_minClientAreaSize = minSize;

	Vector2i newSize(m_clientAreaSize);
	if (m_clientAreaSize.x < m_minClientAreaSize.x)
		newSize.x = m_minClientAreaSize.x;

	if (m_clientAreaSize.y < m_minClientAreaSize.y)
		newSize.y = m_minClientAreaSize.y;

	if (newSize != m_clientAreaSize)
		Resize(newSize);
}

void GameWindow::Show() noexcept
{
	ShowWindow(m_handle, SW_SHOW);
	SetWindowPos(m_handle, (HWND)HWND_TOP, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
}

void GameWindow::Hide() noexcept
{
	ShowWindow(m_handle, SW_HIDE);
}

void GameWindow::Activate() noexcept
{
	EnableWindow(m_handle, true);
	m_isActive = true;
}

void GameWindow::Deactivate() noexcept
{
	EnableWindow(m_handle, false);
	m_isActive = false;
}

void GameWindow::SetIcon(int iconID)
{
	auto icon{ LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(iconID)) };
	if (icon == nullptr)
		throw WINAPI_LAST_EXCEPTION();

	SendMessage(m_handle, WM_SETICON, ICON_BIG, (LPARAM)icon);
	SendMessage(m_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
}

void GameWindow::Rerender()
{
	RunRenderers();
}

Vector2i GameWindow::GetPosition() const noexcept
{
	return m_position;
}

Vector2i GameWindow::GetCenterPosition() const noexcept
{
	return m_position + GetWindowSize() / 2;
}

Vector2i GameWindow::GetSize() const noexcept
{
	return m_clientAreaSize;
}

Vector2i GameWindow::GetMinSize() const noexcept
{
	return m_minClientAreaSize;
}

bool GameWindow::IsVisible() const noexcept
{
	return m_isVisible;
}

bool GameWindow::IsActive() const noexcept
{
	return m_isActive;
}

Graphics& GameWindow::GetGraphics() noexcept
{
	return *m_graphics;
}

Camera& GameWindow::GetCamera() noexcept
{
	return *m_camera;
}

const Camera& GameWindow::GetCamera() const noexcept
{
	return *m_camera;
}

Vector2i GameWindow::GetWindowSize() const noexcept
{
	return m_clientAreaSize + m_windowBordersSize;
}

void GameWindow::RunScripts()
{
	for (auto& script : scripts)
		if (script != nullptr)
			script->Execute();
}

void GameWindow::RunRenderers()
{
	for (auto& renderer : renderers)
		if (renderer != nullptr)
			renderer->Render();

	m_graphics->UpdateTargetWindow();
}

LRESULT GameWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_LBUTTONDOWN:
			OnMouseButtonPressed(Mouse::Button::Left, MAKEPOINTS(lParam));
			return 0;

		case WM_LBUTTONUP:
			OnMouseButtonUnpressed(Mouse::Button::Left, MAKEPOINTS(lParam));
			return 0;

		case WM_RBUTTONDOWN:
			OnMouseButtonPressed(Mouse::Button::Right, MAKEPOINTS(lParam));
			return 0;

		case WM_RBUTTONUP:
			OnMouseButtonUnpressed(Mouse::Button::Right, MAKEPOINTS(lParam));
			return 0;

		case WM_MBUTTONDOWN:
			OnMouseButtonPressed(Mouse::Button::Middle, MAKEPOINTS(lParam));
			return 0;

		case WM_MBUTTONUP:
			OnMouseButtonUnpressed(Mouse::Button::Middle, MAKEPOINTS(lParam));
			return 0;

		case WM_MOUSEWHEEL:
			OnMouseWheelScrolled(GET_WHEEL_DELTA_WPARAM(wParam));
			return 0;

		case WM_KEYDOWN:
			OnKeyPressed(static_cast<unsigned char>(wParam));
			return 0;

		case WM_KEYUP:
			OnKeyUnpressed(static_cast<unsigned char>(wParam));
			return 0;

		case WM_KILLFOCUS:
			OnKillFocus();
			return 0;

		case WM_GETMINMAXINFO:
			OnGetMinMaxInfo(reinterpret_cast<LPMINMAXINFO>(lParam));
			return 0;

		case WM_MOVE:
			OnMovedByDragging();
			return 0;

		case WM_SIZE:
			OnResizedByDraggingBordersOrHid({ LOWORD(lParam), HIWORD(lParam) });
			return 0;

		case WM_CLOSE:
			OnClose();
			return 0;
	}

	return DefWindowProc(m_handle, message, wParam, lParam);
}

void GameWindow::OnMouseButtonPressed(Mouse::Button button, POINTS windowCursorPosition) noexcept
{
	mouse.OnButtonPressed(button, PointsToVector(windowCursorPosition));

	RunScripts();
	RunRenderers();

	mouse.m_buttonPressedStates[(int)button] = false;
}

void GameWindow::OnMouseButtonUnpressed(Mouse::Button button, POINTS windowCursorPosition) noexcept
{
	mouse.OnButtonUnpressed(button, PointsToVector(windowCursorPosition));

	RunScripts();
	RunRenderers();

	mouse.m_buttonUnpressedStates[(int)button] = false;
}

void GameWindow::OnMouseWheelScrolled(int scrollDelta) noexcept
{
	mouse.OnWheelScrolled(scrollDelta);

	RunScripts();
	RunRenderers();

	mouse.m_scrollDelta = 0;
}

void GameWindow::OnKeyPressed(unsigned char keycode) noexcept
{
	keyboard.OnKeyPressed(keycode);

	RunScripts();
	RunRenderers();

	keyboard.m_keyPressedStates[keycode] = false;
}

void GameWindow::OnKeyUnpressed(unsigned char keycode) noexcept
{
	keyboard.OnKeyUnpressed(keycode);

	RunScripts();
	RunRenderers();

	keyboard.m_keyUnpressedStates[keycode] = false;
}

void GameWindow::OnGetMinMaxInfo(MINMAXINFO* minMaxInfo) noexcept
{
	m_minPossibleClientAreaWidth = minMaxInfo->ptMinTrackSize.x - m_windowBordersSize.x;
	m_minClientAreaSize.x = std::max(m_minPossibleClientAreaWidth, m_minClientAreaSize.x);

	minMaxInfo->ptMinTrackSize = VectorToPoint(m_minClientAreaSize + m_windowBordersSize);
}

void GameWindow::OnMovedByDragging() noexcept
{
	m_position = GetWindowPosition(m_handle);
}

void GameWindow::OnResizedByDraggingBordersOrHid(Vector2i newClientAreaSize) noexcept
{
	if (newClientAreaSize == Vector2i(0, 0))
	{
		m_isVisible = false;
	}
	else if (newClientAreaSize == m_clientAreaSize)
	{
		m_isVisible = true;
	}
	else
	{
		m_clientAreaSize = newClientAreaSize;

		m_clientAreaSizeIsChangedRaiser.Raise(m_clientAreaSize);
		RunRenderers();
	}
}

void GameWindow::OnKillFocus() noexcept
{
	keyboard.ResetState();
	mouse.ResetState();
}

void GameWindow::OnClose() noexcept
{
	DestroyWindow(m_handle);
	m_handle = nullptr;

	PostQuitMessage(0);
}

const GameWindow::GameWindowClass GameWindow::GameWindowClass::gameWindowClass;

GameWindow::GameWindowClass::GameWindowClass() : WindowClass(L"GameWindowClass")
{
	WNDCLASSEX windowClass{ sizeof(windowClass) };
	windowClass.lpszClassName = GetName();
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.lpfnWndProc = SetupMessageHandling;
	windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

	if (!RegisterClassEx(&windowClass))
		throw WINAPI_LAST_EXCEPTION();
}
