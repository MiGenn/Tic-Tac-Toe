#include "Camera.h"
#include <stdexcept>
#include <limits>

Camera::Camera(Vector2i canvasSize, Event<Vector2i>& canvasSizeIsChanged) :
	m_canvasSize(canvasSize)
{
	canvasSizeIsChanged += [this](Vector2i newCanvasSize)
	{ 
		m_canvasSize = newCanvasSize;
		UpdatePixelsPerWorldUnit(); 
	};
}

void Camera::Move(Vector2f translation) noexcept
{
	position += translation;
}

void Camera::SetZoom(float zoom)
{
	if (zoom < FLT_MIN)
		zoom = FLT_MIN;

	m_zoom = zoom;
	m_scale = std::sqrt(m_zoom);

	UpdatePixelsPerWorldUnit();
}

void Camera::SetViewedWorldAreaSize(Vector2f viewedWorldAreaSize)
{
	if (viewedWorldAreaSize.x < 0 || viewedWorldAreaSize.y < 0)
		throw std::logic_error("Viewed world area size is incorrect");

	m_viewedWorldAreaSize = viewedWorldAreaSize;
	UpdatePixelsPerWorldUnit();
}

void Camera::SetMode(Mode mode) noexcept
{
	m_mode = mode;
	UpdatePixelsPerWorldUnit();
}

float Camera::GetZoom() const noexcept
{
	return m_zoom;
}

Vector2f Camera::GetViewedWorldAreaSize() const noexcept
{
	return m_viewedWorldAreaSize;
}

Vector2f Camera::ConvertScreenToWorldPoint(Vector2f screenPoint) const noexcept
{
	screenPoint.x -= m_canvasSize.x * 0.5f;
	screenPoint.y -= m_canvasSize.y * 0.5f;
	screenPoint.y = -screenPoint.y;

	return ConvertScreenToWorldUnits(screenPoint) + position;
}

Box2f Camera::ConvertScreenToWorldBox(const Box2f& screenBox) const noexcept
{
	return { ConvertScreenToWorldPoint(screenBox.topLeft), ConvertScreenToWorldPoint(screenBox.bottomRight) };
}

Vector2f Camera::ConvertWorldToScreenPoint(Vector2f worldPoint) const noexcept
{
	auto screenPoint{ ConvertWorldToScreenUnits(worldPoint - position) };
	screenPoint.y = -screenPoint.y;
	screenPoint.x += m_canvasSize.x * 0.5f;
	screenPoint.y += m_canvasSize.y * 0.5f;

	return screenPoint;
}

Box2f Camera::ConvertWorldToScreenBox(const Box2f& worldBox) const noexcept
{
	return { ConvertWorldToScreenPoint(worldBox.topLeft), ConvertWorldToScreenPoint(worldBox.bottomRight) };
}

Vector2f Camera::ConvertScreenToWorldUnits(Vector2f screenUnits) const noexcept
{
	return { screenUnits.x / m_pixelsPerWorldUnit.x, screenUnits.y / m_pixelsPerWorldUnit.y };
}

Vector2f Camera::ConvertWorldToScreenUnits(Vector2f worldUnits) const noexcept
{
	return { worldUnits.x * m_pixelsPerWorldUnit.x, worldUnits.y * m_pixelsPerWorldUnit.y };
}

float Camera::ConvertWorldToScreenThickness(float worldThickness) const noexcept
{
	const auto screenThickness{ ConvertWorldToScreenUnits({ worldThickness, worldThickness }) };
	return std::min(screenThickness.x, screenThickness.y);
}

float Camera::ConvertScreenToWorldThickness(float screenThickness) const noexcept
{
	const auto worldThickness{ ConvertScreenToWorldUnits({ screenThickness, screenThickness }) };
	return std::max(worldThickness.x, worldThickness.y);
}

void Camera::UpdatePixelsPerWorldUnit() noexcept
{
	m_pixelsPerWorldUnit = Vector2f(m_canvasSize.x / m_viewedWorldAreaSize.x, m_canvasSize.y / m_viewedWorldAreaSize.y) * m_scale;
	if (m_mode == Mode::FreeAspect)
		m_pixelsPerWorldUnit.x = m_pixelsPerWorldUnit.y = std::min(m_pixelsPerWorldUnit.x, m_pixelsPerWorldUnit.y);
}
