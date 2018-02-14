#include "Car.h"
#include "Camera.h"


Camera::Camera(Car* target)
{
	m_target = target;
	m_up = glm::vec3(0, 1, 0);
	m_position = target->transform.pos;
	target_forward = target->transform.getFront();
	m_forward = target_forward;
	setProjection(0.1f, 200.0f, glm::radians(100.0f));
	m_pitch = 1.6;
	m_yaw = 0;
	m_rotationSpeed = 1.5f;
}


Camera::~Camera()
{
}


void Camera::setProjection(float zNear, float zFar, float fov)
{
	float aspectRatio = WindowHandler::getInstance().getAspectRatio();
	m_projectionMatrix = glm::perspective(fov, aspectRatio, zNear, zFar);
}

void Camera::updatePosition()
{
	//m_position = m_target->transform.pos;
}

void Camera::updateAngle(glm::vec2 mouseDelta)
{
	float xRot = mouseDelta.x * m_rotationSpeed;
	float yRot = mouseDelta.y * m_rotationSpeed;

	m_yaw += xRot;
	if(m_pitch + yRot < 3 && m_pitch + yRot > 0.2f)
		m_pitch += yRot;


	m_forward.x = glm::cos(m_yaw) * glm::sin(m_pitch);
	m_forward.y = glm::cos(m_pitch);
	m_forward.z = glm::sin(m_yaw) * glm::sin(m_pitch);

	m_right = -glm::cross(m_forward, m_up);
	glm::normalize(m_right);

}


glm::mat4 Camera::getViewMatrix()
{
	target_forward = m_target->transform.getFront();
	glm::vec3 carSeatOffset = glm::vec3(m_target->transform.getModelMatrix() * glm::vec4(1, 1.8, -2, 1));

	glm::vec3 pos = m_position + carSeatOffset;
	glm::vec3 forward = target_forward + m_forward;

	return glm::lookAt(pos, pos + m_forward, m_up);
}


glm::mat4 Camera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

glm::vec3 Camera::getPosition()
{
	return m_position;
}