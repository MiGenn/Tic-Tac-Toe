#pragma once
#include "MemoryDC.h"
#include "UniqueHGDIOBJ.h"
#include "SelectionWrapper.h"
#include "Vector2D.h"

class Canvas final
{
public:
	Canvas(HDC referenceDC, Vector2i size);
	Canvas(const Canvas&) = delete;

	Canvas& operator=(const Canvas&) = delete;

	void Resize(HDC referenceDC, Vector2i size);

	HDC GetDC() noexcept;
	Vector2i GetSize() const noexcept;

private:
	Vector2i m_clientAreaSize;

	MemoryDC m_deviceContext;
	UniqueHGDIOBJ<HBITMAP> m_bitmap;
	SelectionWrapper<HBITMAP> m_bitmapSelectionWrapper;
};
