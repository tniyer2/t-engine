
#ifndef GRAPHICS_SHADER_ID_H
#define GRAPHICS_SHADER_ID_H

#include "core/id.h"

namespace TEngine::Graphics {
	struct shader_tag {};
	using shader = TEngine::Core::Id<shader_tag, unsigned int, 0>;
}
#endif
