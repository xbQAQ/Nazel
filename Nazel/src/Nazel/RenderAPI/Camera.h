#pragma once
#include "glm/glm.hpp"

namespace Nazel {
class Camera
{
public:
	Camera(float Fov, float Near, float Far, float Aspect);
	glm::mat4x4 GetView() const;
	glm::mat4x4 GetProjection() const;
	glm::mat4x4 GetProjectionView() const;

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& Axis, float Angle);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;

private:
	void RecalculateViewMatrix();
private:
	glm::vec3 m_Position;
	glm::vec3 m_Axis;
	float m_Angle;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ProjectionViewMatrix;
};
}

