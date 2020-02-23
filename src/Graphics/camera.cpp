
#include "camera.h";

namespace TEngine { namespace Graphics {
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)),
	movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM),
	position(position), worldUp(up),
	yaw(yaw), pitch(pitch) {

	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)),
	movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM),
	yaw(yaw), pitch(pitch) {

	position = glm::vec3(posX, posY, posZ);
	worldUp = glm::vec3(upX, upY, upZ);
	updateCameraVectors();
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	glm::vec3 dir;
	switch (direction) {
		case Camera_Movement::FORWARD:
			dir = front;
			break;
		case Camera_Movement::BACKWARD:
			dir = -front;
			break;
		case Camera_Movement::LEFT:
			dir = -right;
			break;
		case Camera_Movement::RIGHT:
			dir = right;
			break;
		default:
			dir = glm::vec3(0, 0, 0);
	}
	position += dir * velocity;
	std::cout << "position: " + std::to_string(position.x) + ", " + std::to_string(position.z) + "\n";
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
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

void Camera::updateCameraVectors() {
	// Calculate the Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	// Calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
}}
