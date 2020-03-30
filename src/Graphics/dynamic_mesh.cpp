
#include "dynamic_mesh.h"
#include <iostream>

namespace TEngine::Graphics {

Mesh DynamicMesh::glBind() {
	unsigned int VAO, VBO, EBO;

	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	Vertex::glEnableAttribs();

	// unbind
	glBindVertexArray(0);

	Mesh m = { VAO, indices.size(), textures };
	return m;
}
}
