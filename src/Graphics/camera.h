
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

using glm::mat4;
using glm::vec3;

namespace TEngine { namespace Graphics {
enum class Camera_Movement {
	FORWARD, BACKWARD, LEFT, RIGHT
};

const float SPEED = 10.0f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera {
public:
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(mat4);

	mat4 getViewMatrix() {
		return m_matrix;
	}
	vec3 getRight() { return normalize(m_matrix[0]); }
	vec3 getUp() { return normalize(m_matrix[1]); }
	vec3 getForward() { return normalize(m_matrix[2]); }
	void processKeyboard(Camera_Movement, float);
	void processMouseMovement(float, float, GLboolean=true);
	void processMouseScroll(float);

private:
	mat4 m_matrix;
};
}}
#endif
