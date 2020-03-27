
#include "camera.h";
#include "window.h";

namespace TEngine { namespace Graphics {

glm::mat4 Camera::getPerspectiveMatrix() {
	return glm::perspective(
		glm::radians(zoom),
		(float)window->getWidth() / (float)window->getHeight(),
		0.1f,
		100.0f);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
	float speed = movementSpeed * deltaTime;
	glm::mat4 m = inverse(getViewMatrix());
	glm::vec4 front = m * glm::vec4(0, 0, 1, 0);
	glm::vec4 right = m * glm::vec4(1, 0, 0, 0);
	glm::vec4 dirVec;
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
			dirVec = glm::vec4();
	}
	m_matrix = glm::translate(m_matrix, glm::vec3(dirVec * speed));
}

void Camera::processMouseMovement(float posx, float posy, GLboolean constrainPitch) {
	if (m_lastMouseX == -1) {
		m_lastMouseX = posx;
		m_lastMouseY = posy;
	}
	float offsetX = posx - m_lastMouseX;
	float offsetY = m_lastMouseY - posy;
	m_lastMouseX = posx;
	m_lastMouseY = posy;

	offsetX *= mouseSensitivity;
	offsetY *= mouseSensitivity;

	m_matrix = glm::rotate(m_matrix, offsetX, glm::vec3(getUp()));
	// m_matrix = rotate(m_matrix, yoffset, getRight());
}

void Camera::processMouseScroll(float offsetY) {
	if (zoom >= 1.0f && zoom <= 45.0f) {
		zoom -= offsetY;
	}
	if (zoom <= 1.0f) {
		zoom = 1.0f;
	}
	if (zoom >= 45.0f) {
		zoom = 45.0f;
	}
}
}}

/*
void Window::processInput(float deltaTime) {
	if (input.isKeyDown("escape")) {
		glfwSetWindowShouldClose(m_windowPtr, true);
	}

	if (input.isKeyDown("w")) {
		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
	}
	if (input.isKeyDown("s")) {
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	}
	if (input.isKeyDown("a")) {
		camera.processKeyboard(CameraMovement::LEFT, deltaTime);
	}
	if (input.isKeyDown("d")) {
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
	}

	MouseEvent e = input.getMouseInfo();
	camera.processMouseMovement(e.posX, e.posY);

	ScrollEvent e2 = input.getScrollInfo();
	camera.processMouseScroll(e2.yoffset);

	if (input.isKeyDown("mouse_left")) {
		std::cout << "left clicking\n";
	}
	if (input.isKeyDown("mouse_right")) {
		std::cout << "right clicking\n";
	}
	if (input.isKeyDown("mouse_middle")) {
		std::cout << "middle clicking\n";
	}
}
*/
