#include "Canvas.h"
#include <stdexcept>

Canvas::Canvas(HDC referenceDC, Vector2i size)
{
	Resize(referenceDC, size);
}

void Canvas::Resize(HDC referenceDC, Vector2i size)
{
	if (size.x <= 0 || size.y <= 0)
		throw std::logic_error("Size is incorrect");

	m_bitmapSelectionWrapper.~SelectionWrapper();
	m_deviceContext.Reset(CreateCompatibleDC(referenceDC));
	if (m_deviceContext.Get() == nullptr)
		throw std::runtime_error("Reference Device Context is invalid");

	m_clientAreaSize = size;
	m_bitmap.Reset(CreateCompatibleBitmap(referenceDC, m_clientAreaSize.x, m_clientAreaSize.y));
	m_bitmapSelectionWrapper.Select(m_deviceContext.Get(), m_bitmap.Get());
}

HDC Canvas::GetDC() noexcept
{
	return m_deviceContext.Get();
}

Vector2i Canvas::GetSize() const noexcept
{
	return m_clientAreaSize;
}
