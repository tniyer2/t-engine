
#include "camera.h";
#include "window.h";

namespace TEngine { namespace Graphics {
Camera::Camera() : m_matrix(mat4()) {}

mat4 Camera::getPerspectiveMatrix() {
	return glm::perspective(
		glm::radians(zoom),
		(float)window->getWidth() / (float)window->getHeight(),
		0.1f,
		100.0f);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
	float speed = movementSpeed * deltaTime;
	mat4 m = inverse(getViewMatrix());
	vec4 front = m * vec4(0, 0, 1, 0);
	vec4 right = m * vec4(1, 0, 0, 0);
	vec4 dirVec;
	switch (direction) {
		case CameraMovement::FORWARD:
			dirVec = front;
			break;
		case CameraMovement::BACKWARD:
			dirVec = -front;
			break;
		case CameraMovement::LEFT:
			dirVec = right;
			break;
		case CameraMovement::RIGHT:
			dirVec = -right;
			break;
		default:
			dirVec = vec4();
	}
	m_matrix = translate(m_matrix, vec3(dirVec * speed));
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

	m_matrix = rotate(m_matrix, xoffset, vec3(getUp()));
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
