
#ifndef GRAPHICS_MODEL_H
#define GRAPHICS_MODEL_H

#include "mesh.h"
#include "dynamic_mesh.h"
#include "vertex.h"
#include "texture.h"
#include "shader.h"
#include "core/prefab.h"
#include "core/resource_handle.h"
#include "core/resource_manager.h"

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace TEngine::Graphics {

unsigned int textureFromFile(const char*, const std::string&, bool=false);

class Model {
private:
	std::string m_path;
	std::string m_directory;
	bool m_gammaCorrection;
public:
	Model(const std::string&, bool=false);
	Core::Prefab* loadModel();
private:
	void processNode(Core::PrefabNode*, const aiScene*, aiNode*, aiMesh*);
	Core::ResourceHandle<Mesh> processMesh(const aiScene*, aiMesh*);

	void loadMaterialTextures(TextureArray&, unsigned int&, aiMaterial*, aiTextureType);
};
}
#endif
