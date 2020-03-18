
#ifndef CORE_ENTITY_H
#define CORE_ENTITY_H

#include "id.h"

namespace TEngine::Core {

struct entity_tag {};
using entity = Id<entity_tag, unsigned int, 0>;
}
#endif
