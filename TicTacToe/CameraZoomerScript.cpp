#include "CameraZoomerScript.h"
#include <algorithm>
#include <cmath>

CameraZoomerScript::CameraZoomerScript(GameWindow& gameWindow) noexcept : Script(gameWindow)
{

}

void CameraZoomerScript::Execute()
{
	const auto scrollDelta{ m_mouse.GetScrollDelta() };
	if (scrollDelta != 0)
	{
		auto newZoom{ m_camera.GetZoom() };
		auto zoomToAdd{ m_zoomSensitivity * scrollDelta };

		CalculateZoomForSmallZoom(newZoom, zoomToAdd);
		CalculateZoomForBigZoom(newZoom, zoomToAdd);
		CalculateZoomForSmallZoom(newZoom, zoomToAdd);

		newZoom = std::clamp(newZoom, m_minZoom, m_maxZoom);
		m_camera.SetZoom(newZoom);
	}
}

void CameraZoomerScript::CalculateZoomForSmallZoom(float& currentZoom, float& zoomToAdd) const noexcept
{
	if (currentZoom < 1.f || (currentZoom == 1.f && zoomToAdd < 0.f))
	{
		const auto invertedCurrentZoom{ 1.f / currentZoom };
		const auto newInvertedZoom{ invertedCurrentZoom - zoomToAdd };

		if (newInvertedZoom >= 1.f)
		{
			currentZoom = 1.f / newInvertedZoom;
			zoomToAdd = 0.f;
		}
		else
		{
			zoomToAdd -= invertedCurrentZoom - 1.f;
			currentZoom = 1.f;
		}
	}
}

void CameraZoomerScript::CalculateZoomForBigZoom(float& currentZoom, float& zoomToAdd) const noexcept
{
	if (currentZoom > 1.f || (currentZoom == 1.f && zoomToAdd > 0.f))
	{
		const auto newZoom{ currentZoom + currentZoom * zoomToAdd };
		if (newZoom >= 1.f)
		{
			currentZoom = newZoom;
			zoomToAdd = 0.f;
		}
		else
		{
			currentZoom = 1.f;
			zoomToAdd -= (1.f - currentZoom) / currentZoom;
		}
	}
}

