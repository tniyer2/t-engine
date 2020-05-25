
#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include "core/typed_component_interface.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

namespace TEngine::Graphics {

class Camera : public Core::TypedComponentInterface<Camera> {
public:
	float fieldOfView = 45.0f; // degrees
	float near = 0.1f;
	float far = 100.0f;

	using TypedComponentInterface<Camera>::TypedComponentInterface;

	glm::mat4 getPerspectiveMatrix(float, float);
};
}
#endif
