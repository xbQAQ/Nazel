#pragma once
#include "Nazel/Events/Event.h"
#include "Nazel/Events/MouseEvent.h"
#include "Nazel/Events/ApplicationEvent.h"
#include "Nazel/Core/TimeStep.h"
#include "Nazel/RenderAPI/OrthographicCamera.h"

namespace Nazel {
class OrthographicCameraController
{
public:
	OrthographicCameraController(float aspectRatio, bool rotation = false);

	void OnUpdate(TimeStep ts);
	void OnEvent(Event& e);

	OrthographicCamera& GetCamera() { return m_Camera; }
	const OrthographicCamera& GetCamera() const { return m_Camera; }

	float GetZoomLevel() const { return m_ZoomLevel; }
	void SetZoomLevel(float level) { m_ZoomLevel = level; }

private:
	bool OnMouseScrolled(MouseScrolledEvent& e);
	bool OnWindowResized(WindowResizeEvent& e);


private:
	float m_AspectRatio;
	float m_ZoomLevel = 1.0f;	// 跟应用的高是1:1的关系，相当于高
	OrthographicCamera m_Camera;

	bool m_rotation;

	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_CameraRotation = 0.0f;
	float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;

};
}

