
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.h"

using namespace std;

namespace TEngine { namespace Graphics {
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	Mesh(vector<Vertex>, vector<unsigned int>, vector<Texture>);
	void draw(Shader);

private:
	unsigned int m_VBO, m_EBO;
	void setupMesh();
};
}}
#endif
