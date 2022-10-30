#include "FieldMoverScript.h"

FieldMoverScript::FieldMoverScript(GameWindow& gameWindow) noexcept : Script(gameWindow)
{

}

void FieldMoverScript::Execute()
{
	Vector2f translation;

	if (m_keyboard.IsKeyPressed('W'))
		translation.y -= m_translation;

	if (m_keyboard.IsKeyPressed('A'))
		translation.x += m_translation;

	if (m_keyboard.IsKeyPressed('S'))
		translation.y += m_translation;

	if (m_keyboard.IsKeyPressed('D'))
		translation.x -= m_translation;

	m_camera.Move(translation);

	if (m_keyboard.IsKeyHeld(VK_SHIFT) && m_keyboard.IsKeyPressed('C'))
		m_camera.position = { 0.f, 0.f };
}
