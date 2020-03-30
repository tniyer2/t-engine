
#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include <assimp/Importer.hpp>
#include <string>
#include <array>

namespace TEngine::Graphics {

enum class TextureType {
	none,
	diffuse,
	specular,
	normal,
	height
};

inline static const std::array<std::string, 5> TEXTURE_TYPE_NAMES = {
	"none",
	"diffuse",
	"specular",
	"normal",
	"height"
};

struct Texture {
	unsigned int id = 0;
	TextureType type = TextureType::none;
};
}
#endif
