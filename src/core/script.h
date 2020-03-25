
#ifndef CORE_SCRIPT_H
#define CORE_SCRIPT_H

#include "entity.h"
#include "read_only_property.h"

namespace TEngine::Core {

class Script {
public:
	ReadOnlyProperty<entity> entityId;

	Script() { }
	Script(entity id) : entityId(id) { }

	virtual void Awake() { }
	virtual void Update(float) { }
};
}
#endif
