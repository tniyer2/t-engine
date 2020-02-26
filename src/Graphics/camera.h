
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

using glm::mat4;
using glm::vec3;

namespace TEngine { namespace Graphics {

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	float movementSpeed = 10.0f;
	float mouseSensitivity = 0.05f;
	float zoom = 45.0f;

	Camera();
	Camera(mat4);

	inline mat4 getViewMatrix() { return m_matrix; }
	inline vec3 getRight() { return normalize(m_matrix[0]); }
	inline vec3 getUp() { return normalize(m_matrix[1]); }
	inline vec3 getForward() { return normalize(m_matrix[2]); }

	void processKeyboard(CameraMovement, float);
	void processMouseMovement(float, float, GLboolean=true);
	void processMouseScroll(float);
private:
	mat4 m_matrix;
	float m_lastMouseX = -1;
	float m_lastMouseY = -1;
};
}}
#endif
