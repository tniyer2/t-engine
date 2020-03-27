
#ifndef GRAPHICS_MODEL_H
#define GRAPHICS_MODEL_H

#include "mesh.h"
#include "shader.h"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <map>
#include <vector>

#include <fstream>
#include <sstream>
#include <iostream>

namespace TEngine { namespace Graphics {

unsigned int textureFromFile(const char*, const std::string&, bool=false);

class Model {
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(const std::string&, bool=false);
	void draw(Shader shader);

private:
	void loadModel(std::string const&);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, std::string);
};
}}
#endif
