#pragma once
#include <memory>
#include "WindowClass.h"
#include "Window.h"
#include "Event.h"
#include "Graphics.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"

class GameWindow final : public Window
{
public:
	Keyboard keyboard;
	Mouse mouse;

	std::vector<std::unique_ptr<class Script>> scripts;
	std::vector<std::unique_ptr<class Renderer>> renderers;

	GameWindow(const std::wstring& title, Vector2i size, bool canBeResizedByDraggingBorders);
	GameWindow(const GameWindow&) = delete;
	virtual ~GameWindow() noexcept;

	GameWindow& operator=(const GameWindow&) = delete;

	void SetPosition(Vector2i position);
	void SetCenterPosition(Vector2i centerPosition);
	/// <summary>
	/// Вікно не змінює центральну позицію
	/// </summary>
	void Resize(Vector2i size);
	void SetMinSize(Vector2i minSize);

	void Show() noexcept;
	void Hide() noexcept;
	void Activate() noexcept;
	void Deactivate() noexcept;
	void SetIcon(int iconID);
	void Rerender();

	Vector2i GetPosition() const noexcept;
	Vector2i GetCenterPosition() const noexcept;
	Vector2i GetSize() const noexcept;
	Vector2i GetMinSize() const noexcept;
	bool IsVisible() const noexcept;
	bool IsActive() const noexcept;

	Graphics& GetGraphics() noexcept;
	Camera& GetCamera() noexcept;
	const Camera& GetCamera() const noexcept;

private:
	EventRaiser<Vector2i> m_clientAreaSizeIsChangedRaiser;
	Event<Vector2i> m_clientAreaSizeIsChanged;

	std::unique_ptr<Graphics> m_graphics;
	std::unique_ptr<Camera> m_camera;

	Vector2i m_position;
	Vector2i m_clientAreaSize;
	Vector2i m_minClientAreaSize{ 1, 1 };
	bool m_isVisible{ false };
	bool m_isActive{ true };

	DWORD m_windowStyle;
	Vector2i m_windowBordersSize;
	int m_minPossibleClientAreaWidth{ 0 };	

	Vector2i GetWindowSize() const noexcept;

	void RunScripts();
	void RunRenderers();

	LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	
	void OnMouseButtonPressed(Mouse::Button button, POINTS windowCursorPosition) noexcept;
	void OnMouseButtonUnpressed(Mouse::Button button, POINTS windowCursorPosition) noexcept;
	void OnMouseWheelScrolled(int scrollDelta) noexcept;
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyUnpressed(unsigned char keycode) noexcept;

	void OnGetMinMaxInfo(MINMAXINFO* minMaxInfo) noexcept;
	void OnMovedByDragging() noexcept;
	void OnResizedByDraggingBordersOrHid(Vector2i newClientAreaSize) noexcept;

	void OnKillFocus() noexcept;

	void OnClose() noexcept;

	class GameWindowClass final : public WindowClass
	{
	public:
		static const GameWindowClass gameWindowClass;

	private:
		GameWindowClass();
		GameWindowClass(const GameWindowClass&) = delete;
		GameWindowClass& operator=(const GameWindowClass&) = delete;
	};
};
