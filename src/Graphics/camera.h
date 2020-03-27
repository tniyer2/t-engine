
#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace TEngine { namespace Graphics {

class Window;

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	Window* window;
	float movementSpeed = 10.0f;
	float mouseSensitivity = 0.05f;
	float zoom = 45.0f;

	glm::mat4 getPerspectiveMatrix();
	glm::mat4 getViewMatrix() { return m_matrix; }
	glm::vec4 getRight() { return glm::normalize(m_matrix[0]); }
	glm::vec4 getUp() { return glm::normalize(m_matrix[1]); }
	glm::vec4 getForward() { return glm::normalize(m_matrix[2]); }

	void processKeyboard(CameraMovement, float);
	void processMouseMovement(float, float, GLboolean=true);
	void processMouseScroll(float);
private:
	glm::mat4 m_matrix;
	float m_lastMouseX = -1;
	float m_lastMouseY = -1;
};
}}
#endif
