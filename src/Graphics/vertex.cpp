
#include "vertex.h"
#include <glad/glad.h>

namespace TEngine::Graphics {

void Vertex::glEnableAttribs() {
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, 2, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(
		4, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
}
}
