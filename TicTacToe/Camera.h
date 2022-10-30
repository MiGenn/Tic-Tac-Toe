#pragma once
#include "Vector2D.h"
#include "Graphics.h"

class Camera final
{
public:
	enum class Mode
	{
		FreeAspect,
		Stretch
	};

	Vector2f position;

	Camera(Vector2i canvasSize, Event<Vector2i>& canvasSizeIsChanged);
	Camera(const Camera&) = delete;

	Camera& operator=(const Camera&) = delete;

	void Move(Vector2f translation) noexcept;
	void SetZoom(float zoom);
	void SetViewedWorldAreaSize(Vector2f viewedWorldAreaSize);
	void SetMode(Mode mode) noexcept;

	float GetZoom() const noexcept;
	Vector2f GetViewedWorldAreaSize() const noexcept;

	Vector2f ConvertScreenToWorldPoint(Vector2f screenPoint) const noexcept;
	Box2f ConvertScreenToWorldBox(const Box2f& screenBox) const noexcept;
	Vector2f ConvertWorldToScreenPoint(Vector2f worldPoint) const noexcept;
	Box2f ConvertWorldToScreenBox(const Box2f& worldBox) const noexcept;

	Vector2f ConvertScreenToWorldUnits(Vector2f screenUnits) const noexcept;
	Vector2f ConvertWorldToScreenUnits(Vector2f worldUnits) const noexcept;

	float ConvertWorldToScreenThickness(float worldThickness) const noexcept;
	float ConvertScreenToWorldThickness(float screenThickness) const noexcept;

private:
	Vector2i m_canvasSize;
	Vector2f m_pixelsPerWorldUnit;
	float m_scale{ 1.f };

	float m_zoom{ 1.f };
	Vector2f m_viewedWorldAreaSize{ 10.f, 10.f };
	Mode m_mode{ Mode::FreeAspect };

	void UpdatePixelsPerWorldUnit() noexcept;
};
