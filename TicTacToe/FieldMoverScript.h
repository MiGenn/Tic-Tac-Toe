#pragma once
#include "Script.h"
#include "GameWindow.h"

class FieldMoverScript final : public Script
{
public:
	FieldMoverScript(GameWindow& gameWindow) noexcept;
	FieldMoverScript(const FieldMoverScript&) = delete;

	FieldMoverScript& operator=(const FieldMoverScript&) = delete;

	virtual void Execute() override;

private:
	static constexpr float m_translation{ 1.f };
};
