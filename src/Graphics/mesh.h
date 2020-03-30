
#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include "texture.h"
#include <array>

namespace TEngine::Graphics {

constexpr static size_t TEXTURES_PER_MESH = 16;
using TextureArray = std::array<Texture, TEXTURES_PER_MESH>;

struct Mesh {
	unsigned int VAO = 0;
	size_t numIndices = 0;
	TextureArray textures;
};
}
#endif
