#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "WindowHandler.h"

class Car;

class Camera
{
public:
	Camera(Car* target);
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::vec3 getPosition();

	void setProjection(float zNear, float zFar, float fov);

	void updatePosition();
	void updateAngle(glm::vec2 mouseDelta);

private:
	float m_rotationSpeed, m_pitch, m_yaw;
	
	Car* m_target;
	
	glm::vec3 m_position, 
		m_up, 
		m_forward, 
		m_right,
		target_forward,
		mouseRotation;

	glm::mat4 m_projectionMatrix;
};

