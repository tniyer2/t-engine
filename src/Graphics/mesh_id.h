
#ifndef GRAPHICS_MESH_ID_H
#define GRAPHICS_MESH_ID_H

#include "../core/id.h"

namespace TEngine::Graphics {

struct mesh_tag{};
using meshId = TEngine::Core::Id<mesh_tag, unsigned int, 0>;
}
#endif
