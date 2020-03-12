
#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include "../core/id.h"
#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace TEngine::Graphics {

struct mesh_tag{};
using meshId = TEngine::Core::Id<mesh_tag, unsigned int, 0>;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;
private:
	unsigned int m_VBO, m_EBO;
public:
	Mesh(std::vector<Vertex>, std::vector<unsigned int>, std::vector<Texture>);
	void draw(const Shader&);
private:
	void setupMesh();
};
}
#endif
