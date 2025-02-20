#include "pch.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Nazel {
Camera::Camera(float Fov, float Near, float Far, float Aspect): 
	m_ProjectionMatrix(glm::perspective(Fov, Aspect, Near, Far)), m_ViewMatrix(1.0f) {
	m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
glm::mat4x4 Camera::GetView() const {
	return m_ViewMatrix;
}
glm::mat4x4 Camera::GetProjection() const {
	return m_ProjectionMatrix;
}
glm::mat4x4 Camera::GetProjectionView() const {
	return m_ProjectionViewMatrix;
}
void Camera::SetPosition(const glm::vec3& position) {
	m_Position = position;
	RecalculateViewMatrix();
}
void Camera::SetRotation(const glm::vec3& Axis, float Angle) {
	m_Axis = Axis;
	m_Angle = Angle;
	RecalculateViewMatrix();
}
glm::vec3 Camera::GetPosition() const {
	return m_Position;
}
glm::vec3 Camera::GetRotation() const {
	return m_Axis * m_Angle;
}
void Camera::RecalculateViewMatrix() {
	glm::mat4x4 transform = glm::translate(glm::mat4x4(1.0f), m_Position) * glm::rotate(glm::mat4x4(1.0f), m_Angle, m_Axis);
	m_ViewMatrix = glm::inverse(transform);
	m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
}
