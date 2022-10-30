#pragma once
#include "Script.h"
#include "GameWindow.h"

class CameraZoomerScript final : public Script
{
public:
	CameraZoomerScript(GameWindow& gameWindow) noexcept;
	CameraZoomerScript(const CameraZoomerScript&) = delete;

	CameraZoomerScript& operator=(const CameraZoomerScript&) = delete;

	virtual void Execute() override;

private:
	static constexpr float m_zoomSensitivity{ 0.001f };
	static constexpr float m_minZoom{ 0.2f };
	static constexpr float m_maxZoom{ 20.f };

	void CalculateZoomForSmallZoom(float& currentZoom, float& zoomToAdd) const noexcept;
	void CalculateZoomForBigZoom(float& currentZoom, float& zoomToAdd) const noexcept;
};
