
#include "model.h"
#include "mesh_component.h"

namespace TEngine::Graphics {

glm::mat4 convertAiMatrix(aiMatrix4x4 matrix) {
	return {
		{ matrix.a1, matrix.a2, matrix.a3, matrix.a4 },
		{ matrix.b1, matrix.b2, matrix.b3, matrix.b4 },
		{ matrix.c1, matrix.c2, matrix.c3, matrix.c4 },
		{ matrix.d1, matrix.d2, matrix.d3, matrix.d4 }
	};
}

glm::vec3 convertAiVec3(aiVector3D vector) {
	return { vector.x, vector.y, vector.z };
}

glm::vec2 convertAiVec2(aiVector2D vector) {
	return { vector.x, vector.y };
}

TextureType convertAiTextureType(aiTextureType type) {
	switch (type) {
		case aiTextureType_DIFFUSE:
			return TextureType::diffuse;
		case aiTextureType_SPECULAR:
			return TextureType::specular;
		case aiTextureType_HEIGHT:
			return TextureType::normal;
		case aiTextureType_AMBIENT:
			return TextureType::height;
		default:
			return TextureType::none;
	}
}

Model::Model(std::string const& path, bool gamma)
	: m_path(path), m_directory(path.substr(0, path.find_last_of('/'))), m_gammaCorrection(gamma) { }

Core::Prefab* Model::loadModel() {
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene =
		importer.ReadFile(m_path,
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	Core::PrefabNode* pRootNode = new Core::PrefabNode();
	Core::Prefab* prefab = new Core::Prefab(pRootNode);
	processNode(pRootNode, scene, scene->mRootNode, nullptr);

	return prefab;
}

void Model::processNode(Core::PrefabNode* pNode, const aiScene* scene, aiNode* node, aiMesh* meshId) {
	if (meshId) {
		auto mesh = processMesh(scene, meshId);
		MeshComponent* meshC = new MeshComponent();
		meshC->mesh = mesh;
		pNode->components.push_back(meshC);
	}

	if (node) {
		pNode->matrix = convertAiMatrix(node->mTransformation);

		for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
			aiMesh* meshId = scene->mMeshes[node->mMeshes[i]];

			auto childNode = new Core::PrefabNode();
			pNode->children.push_back(childNode);
			processNode(childNode, scene, nullptr, meshId);
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i) {
			auto childNode = new Core::PrefabNode();
			pNode->children.push_back(childNode);
			processNode(childNode, scene, node->mChildren[i], nullptr);
		}
	}
}

Core::ResourceHandle<Mesh> Model::processMesh(const aiScene* scene, aiMesh* meshId) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture temp;
	TextureArray textures;

	// Fill vertices
	for (unsigned int i = 0; i < meshId->mNumVertices; i++) {
		Vertex vertex;

		vertex.position = convertAiVec3(meshId->mVertices[i]);
		vertex.normal = convertAiVec3(meshId->mNormals[i]);

		if (meshId->mTextureCoords[0]) {
			vertex.texCoords = convertAiVec3(meshId->mTextureCoords[0][i]);
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		
		vertex.tangent = convertAiVec3(meshId->mTangents[i]);
		vertex.bitangent = convertAiVec3(meshId->mBitangents[i]);

		vertices.push_back(vertex);
	}
	
	// Fill indices
	for (unsigned int i = 0; i < meshId->mNumFaces; i++) {
		aiFace face = meshId->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[meshId->mMaterialIndex];
	unsigned int cur = 0;
	loadMaterialTextures(textures, cur, material, aiTextureType_DIFFUSE);
	loadMaterialTextures(textures, cur, material, aiTextureType_SPECULAR);
	loadMaterialTextures(textures, cur, material, aiTextureType_HEIGHT);
	loadMaterialTextures(textures, cur, material, aiTextureType_AMBIENT);

	DynamicMesh dmesh(vertices, indices, textures);
	auto handle = Core::ResourceManager::getInstance().allocate<Mesh>();
	auto mesh = dmesh.glBind();
	*handle = mesh;
	return handle;
}

void Model::loadMaterialTextures(TextureArray& textures, unsigned int& cur, aiMaterial* mat, aiTextureType type) {
	for (unsigned int i = 0; i < mat->GetTextureCount(type) && cur < TEXTURES_PER_MESH; ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);

		Texture texture;
		texture.id = textureFromFile(str.C_Str(), this->m_directory, m_gammaCorrection);
		texture.type = convertAiTextureType(type);
		textures[cur] = texture;

		++cur;
	}
}

unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else
			format = GL_RED;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
}
