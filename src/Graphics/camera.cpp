
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TEngine::Graphics {

glm::mat4 Camera::getPerspectiveMatrix(float width, float height) {
	return glm::perspective(
		glm::radians(fieldOfView), width / height, near, far);
}
}
