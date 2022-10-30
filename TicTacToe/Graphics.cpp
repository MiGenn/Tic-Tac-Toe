#include "Graphics.h"
#include <stdexcept>
#include "WindowDC.h"
#include "UniqueHGDIOBJ.h"
#include "SelectionWrapper.h"
#include "TypeUtilities.h"
#include "WinapiUtilities.h"

Graphics::Graphics(HWND targetWindow, Event<Vector2i>& clienAreaSizeIsChanged) noexcept : 
	m_targetWindow(targetWindow), m_canvas(WindowDC(targetWindow).Get(), GetWindowClientAreaSize(targetWindow))
{
	clienAreaSizeIsChanged += [this](Vector2i newSize) { UpdateCanvas(newSize); };
}

void Graphics::RenderLine(Vector2i startPosition, Vector2i endPosition, int thickness, ColorRGB color)
{
	static constexpr int lineStyle{ PS_SOLID };

	if (thickness <= 0)
		throw std::logic_error("Thickness is incorrect");

	UniqueHGDIOBJ pen(CreatePen(lineStyle, thickness, ColorRGBToColorref(color)));
	RenderLine(startPosition, endPosition, pen.Get());
}

void Graphics::RenderBox(const Box2i& box, int borderThickness, ColorRGB borderColor)
{
	static constexpr int borderStyle{ PS_SOLID };

	if (borderThickness <= 0)
		throw std::logic_error("Border Thickness is incorrect");

	UniqueHGDIOBJ pen(CreatePen(borderStyle, borderThickness, ColorRGBToColorref(borderColor)));
	RenderBox(BoxToRect(box), pen.Get(), reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH)));
}

void Graphics::RenderSolidBox(const Box2i& box, ColorRGB fillColor)
{
	auto rect{ BoxToRect(box) };
	rect.right += 1;
	rect.bottom += 1;

	UniqueHGDIOBJ brush(CreateSolidBrush(ColorRGBToColorref(fillColor)));
	RenderBox(rect, reinterpret_cast<HPEN>(GetStockObject(NULL_PEN)), brush.Get());
}

void Graphics::RenderSolidBox(const Box2i& box, ColorRGB fillColor, int borderThickness, ColorRGB borderColor)
{
	static constexpr int borderStyle{ PS_SOLID };

	if (borderThickness <= 0)
		throw std::logic_error("Border Thickness is incorrect");

	UniqueHGDIOBJ pen(CreatePen(borderStyle, borderThickness, ColorRGBToColorref(borderColor)));
	UniqueHGDIOBJ brush(CreateSolidBrush(ColorRGBToColorref(fillColor)));
	RenderBox(BoxToRect(box), pen.Get(), brush.Get());
}

void Graphics::RenderEllipse(const Box2i& boundingBox, int borderThickness, ColorRGB borderColor)
{
	static constexpr int borderStyle{ PS_SOLID };

	if (borderThickness <= 0)
		throw std::logic_error("Border Thickness is incorrect");

	UniqueHGDIOBJ pen(CreatePen(borderStyle, borderThickness, ColorRGBToColorref(borderColor)));
	RenderEllipse(BoxToRect(boundingBox), pen.Get(), reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH)));
}

void Graphics::RenderSolidEllipse(const Box2i& boundingBox, ColorRGB fillColor)
{
	auto boundingRect{ BoxToRect(boundingBox) };
	boundingRect.right += 1;
	boundingRect.bottom += 1;

	UniqueHGDIOBJ brush(CreateSolidBrush(ColorRGBToColorref(fillColor)));
	RenderEllipse(boundingRect, reinterpret_cast<HPEN>(GetStockObject(NULL_PEN)), brush.Get());
}

void Graphics::RenderSolidEllipse(const Box2i& boundingBox, ColorRGB fillColor, int borderThickness, ColorRGB borderColor)
{
	static constexpr int borderStyle{ PS_SOLID };

	if (borderThickness <= 0)
		throw std::logic_error("Border Thickness is incorrect");

	UniqueHGDIOBJ pen(CreatePen(borderStyle, borderThickness, ColorRGBToColorref(borderColor)));
	UniqueHGDIOBJ brush(CreateSolidBrush(ColorRGBToColorref(fillColor)));
	RenderEllipse(BoxToRect(boundingBox), pen.Get(), brush.Get());
}

void Graphics::RenderText(const Box2i& boundingBox, const Text& text, const TextFormat& format)
{
	auto canvasDC{ m_canvas.GetDC() };

	const UINT convertedFormat{ (UINT)format.alignmentX | (UINT)format.alignmentY | (UINT)format.overflow };
	auto boundingRect{ BoxToRect(boundingBox) };

	UniqueHGDIOBJ font(CreateWinapiFont(text.GetCharacterSize(), text.style, text.GetFontName()));
	SelectionWrapper fontSelection(canvasDC, font.Get());

	SetBkMode(canvasDC, TRANSPARENT);
	SetTextColor(canvasDC, ColorRGBToColorref(text.color));

	DrawText(canvasDC, text.text.c_str(), -1, &boundingRect, convertedFormat);
}

void Graphics::Clear(ColorRGB color)
{
	const auto canvasSize{ m_canvas.GetSize() };
	RenderSolidBox({ 0, 0, canvasSize.x, canvasSize.y }, color);
}

void Graphics::UpdateTargetWindow()
{
	const Vector2i canvasSize{ m_canvas.GetSize() };
	const Vector2i targetWindowSize{ canvasSize };
	WindowDC windowDC(m_targetWindow);

	if (!StretchBlt(windowDC.Get(), 0, 0, targetWindowSize.x, targetWindowSize.y,
		m_canvas.GetDC(), 0, 0, canvasSize.x, canvasSize.y, SRCCOPY))
		throw WINAPI_LAST_EXCEPTION();
}

Vector2i Graphics::GetCanvasSize() const noexcept
{
	return m_canvas.GetSize();
}

void Graphics::RenderLine(Vector2i startPosition, Vector2i endPosition, HPEN pen)
{
	auto canvasDC = m_canvas.GetDC();
	SelectionWrapper penSelection(canvasDC, pen);

	MoveToEx(canvasDC, startPosition.x, startPosition.y, nullptr);
	LineTo(canvasDC, endPosition.x, endPosition.y);
}

void Graphics::RenderBox(const RECT& box, HPEN pen, HBRUSH brush)
{
	auto canvasDC = m_canvas.GetDC();
	SelectionWrapper penSelection(canvasDC, pen);
	SelectionWrapper brushSelection(canvasDC, brush);

	Rectangle(canvasDC, box.left, box.top, box.right, box.bottom);
}

void Graphics::RenderEllipse(const RECT& boundingBox, HPEN pen, HBRUSH brush)
{
	auto canvasDC = m_canvas.GetDC();
	SelectionWrapper penSelection(canvasDC, pen);
	SelectionWrapper brushSelection(canvasDC, brush);

	Ellipse(canvasDC, boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom);
}

void Graphics::UpdateCanvas(Vector2i newSize)
{
	WindowDC windowDC(m_targetWindow);
	m_canvas.Resize(windowDC.Get(), newSize);
}
