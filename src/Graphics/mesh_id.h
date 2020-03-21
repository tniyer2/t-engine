
#ifndef GRAPHICS_MESH_ID
#define GRAPHICS_MESH_ID

#include "../core/id.h"

namespace TEngine::Graphics {

struct mesh_tag{};
using mesh = Core::Id<mesh_tag, unsigned int, 0>;
}
#endif
