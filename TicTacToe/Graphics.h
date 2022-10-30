#pragma once
#include "Canvas.h"
#include "Event.h"
#include "Box2D.h"
#include "Text.h"
#include "TextFormat.h"

class Graphics final
{
public:
	Graphics(HWND targetWindow, Event<Vector2i>& clienAreaSizeIsChanged) noexcept;
	Graphics(const Graphics&) = delete;

	Graphics& operator=(const Graphics&) = delete;

	void RenderLine(Vector2i startPosition, Vector2i endPosition, int thickness, ColorRGB color);

	void RenderBox(const Box2i& box, int borderThickness, ColorRGB borderColor);
	void RenderSolidBox(const Box2i& box, ColorRGB fillColor);
	void RenderSolidBox(const Box2i& box, ColorRGB fillColor, int borderThickness, ColorRGB borderColor);

	void RenderEllipse(const Box2i& boundingBox, int borderThickness, ColorRGB borderColor);
	void RenderSolidEllipse(const Box2i& boundingBox, ColorRGB fillColor);
	void RenderSolidEllipse(const Box2i& boundingBox, ColorRGB fillColor, int borderThickness, ColorRGB borderColor);

	void RenderText(const Box2i& boundingBox, const Text& text, const TextFormat& format);

	void Clear(ColorRGB color = { 0, 0, 0 });

	void UpdateTargetWindow();
	Vector2i GetCanvasSize() const noexcept;

private:
	HWND m_targetWindow;
	Canvas m_canvas;

	void RenderLine(Vector2i startPosition, Vector2i endPosition, HPEN pen);
	void RenderBox(const RECT& rect, HPEN pen, HBRUSH brush);
	void RenderEllipse(const RECT& boundingRect, HPEN pen, HBRUSH brush);

	void UpdateCanvas(Vector2i newSize);
};
