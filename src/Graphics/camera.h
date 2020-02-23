
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

namespace TEngine { namespace Graphics {
enum class Camera_Movement {
	FORWARD, BACKWARD, LEFT, RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	// Euler Angles
	float yaw;
	float pitch;
	// Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(
		glm::vec3 = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 = glm::vec3(0.0f, 1.0f, 0.0f),
		float=YAW, float=PITCH);
	Camera(float, float, float, float, float, float, float, float);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}
	void processKeyboard(Camera_Movement, float);
	void processMouseMovement(float, float, GLboolean=true);
	void processMouseScroll(float);

private:
	// Calculates the front vector from the Euler Angles
	void updateCameraVectors();
};
}}
#endif
