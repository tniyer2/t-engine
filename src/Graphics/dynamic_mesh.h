
#ifndef GRAPHICS_DYNAMIC_MESH_H
#define GRAPHICS_DYNAMIC_MESH_H

#include "mesh.h"
#include "vertex.h"
#include "texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace TEngine::Graphics {

class DynamicMesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	TextureArray textures;

	DynamicMesh(std::vector<Vertex> v, std::vector<unsigned int> i, TextureArray t)
		: vertices(v), indices(i), textures(t) { }
	Mesh glBind();
};
}
#endif
