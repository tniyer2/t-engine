
#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include "core/component.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

namespace TEngine::Graphics {

class Camera : public Core::TComponent<Camera> {
public:
	float fieldOfView = 45.0f; // degrees
	float near = 0.1f;
	float far = 100.0f;

	using TComponent<Camera>::TComponent;

	glm::mat4 getPerspectiveMatrix(float, float);
};
}
#endif
