
#ifndef MODEL_H
#define MODEL_H

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

#include "mesh.h"
#include "shader.h"

namespace TEngine { namespace Graphics {
using namespace std;

unsigned int textureFromFile(const char*, const string&, bool=false);

class Model {
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

	Model(string const&, bool=false);
	void draw(Shader shader);

private:
	void loadModel(string const&);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, string);
};
}}
#endif
