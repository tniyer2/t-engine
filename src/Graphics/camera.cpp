
#include "camera.h";

namespace TEngine { namespace Graphics {
Camera::Camera()
	: m_matrix(mat4()) {}

Camera::Camera(mat4 m)
	: m_matrix(m) {}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	vec3 front = getForward();
	vec3 right = getRight();
	vec3 dirVec;
	switch (direction) {
		case CameraMovement::FORWARD:
			dirVec = front;
			break;
		case CameraMovement::BACKWARD:
			dirVec = -front;
			break;
		case CameraMovement::LEFT:
			dirVec = -right;
			break;
		case CameraMovement::RIGHT:
			dirVec = right;
			break;
		default:
			dirVec = vec3(0, 0, 0);
	}
	m_matrix = translate(m_matrix, dirVec * velocity);
}

void Camera::processMouseMovement(float posx, float posy, GLboolean constrainPitch) {
	if (m_lastMouseX == -1) {
		m_lastMouseX = posx;
		m_lastMouseY = posy;
	}
	float xoffset = posx - m_lastMouseX;
	float yoffset = m_lastMouseY - posy;
	m_lastMouseX = posx;
	m_lastMouseY = posy;

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
