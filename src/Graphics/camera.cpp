
#include "camera.h";

namespace TEngine { namespace Graphics {
Camera::Camera(mat4 m)
	: m_matrix(m), movementSpeed(SPEED),
	mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	vec3 front = getForward();
	vec3 right = getRight();
	vec3 dirVec;
	switch (direction) {
		case Camera_Movement::FORWARD:
			dirVec = front;
			break;
		case Camera_Movement::BACKWARD:
			dirVec = -front;
			break;
		case Camera_Movement::LEFT:
			dirVec = -right;
			break;
		case Camera_Movement::RIGHT:
			dirVec = right;
			break;
		default:
			dirVec = vec3(0, 0, 0);
	}
	m_matrix = translate(m_matrix, dirVec * velocity);
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	m_matrix = rotate(m_matrix, xoffset, getUp());
	// m_matrix = rotate(m_matrix, yoffset, getRight());
}

void Camera::processMouseScroll(float yoffset) {
	if (zoom >= 1.0f && zoom <= 45.0f) {
		zoom -= yoffset;
	}
	if (zoom <= 1.0f) {
		zoom = 1.0f;
	}
	if (zoom >= 45.0f) {
		zoom = 45.0f;
	}
}
}}
