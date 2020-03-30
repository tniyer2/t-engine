
#ifndef GRAPHICS_VERTEX_H
#define GRAPHICS_VERTEX_H

#include <glm/glm.hpp>

namespace TEngine::Graphics {

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	static void glEnableAttribs();
};
}
#endif
