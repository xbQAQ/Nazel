#include "pch.h"
#include "OrthographicCameraController.h"
#include "Nazel/Core/Input.h"

namespace Nazel {
OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) :
	m_AspectRatio(aspectRatio),
	m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
	m_rotation(rotation) {

}
void OrthographicCameraController::OnUpdate(TimeStep ts) {
	PROFILE_FUNCTION();

	if (Input::IsKeyPressed(NZ_KEY_A)) {
		m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
	}
	else if (Input::IsKeyPressed(NZ_KEY_D)){
		m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
	}

	if (Input::IsKeyPressed(NZ_KEY_S)) {
		m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
	}
	else if (Input::IsKeyPressed(NZ_KEY_W)) {
		m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
	}

	if (m_rotation) {
		if (Input::IsKeyPressed(NZ_KEY_Q))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		if (Input::IsKeyPressed(NZ_KEY_E))
			m_CameraRotation += m_CameraRotationSpeed * ts;

		if (m_CameraRotation > 180.0f)
			m_CameraRotation -= 360.0f;
		else if (m_CameraRotation <= -180.0f)
			m_CameraRotation += 360.0f;

		m_Camera.SetRotation(m_CameraRotation);
	}
	m_Camera.SetPosition(m_CameraPosition);
	m_CameraTranslationSpeed = m_ZoomLevel;
}
void OrthographicCameraController::OnEvent(Event& e) {
	PROFILE_FUNCTION();

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_FUNCTION(OrthographicCameraController::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_FUNCTION(OrthographicCameraController::OnWindowResized));
}
bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
	PROFILE_FUNCTION();

	m_ZoomLevel -= e.GetYOffset() * 0.25f;
	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}
bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
	PROFILE_FUNCTION();

	m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}
}