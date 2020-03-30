
#include "mesh_component.h"

namespace TEngine::Graphics {

void MeshComponent::draw(Core::ResourceHandle<Shader> lShader) {
	// bind textures
	unsigned int nDiffuse = 1;
	unsigned int nSpecular = 1;
	unsigned int nNormal = 1;
	unsigned int nHeight = 1;
	unsigned int nNone = 1;

	for (unsigned int i = 0; i < mesh->textures.size(); ++i) {
		if (mesh->textures[i].type == TextureType::none) break;

		// activate texture unit
		glActiveTexture(GL_TEXTURE0 + i);

		// retrieve texture number (the N in diffuse_textureN)
		auto type = mesh->textures[i].type;
		unsigned int number = 0;
		switch (type) {
			case TextureType::diffuse:
				number = nDiffuse++;
			case TextureType::specular:
				number = nSpecular++;
			case TextureType::normal:
				number = nNormal++;
			case TextureType::height:
				number = nHeight++;
			default:
				number = nNone++;
		}

		// set sampler
		std::string name = TEXTURE_TYPE_NAMES[(unsigned int)type] + "_" + std::to_string(number);
		glUniform1i(glGetUniformLocation(lShader->id, name.c_str()), i);
		glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
	}

	// draw mesh
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->numIndices, GL_UNSIGNED_INT, 0);

	// reset
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
}
