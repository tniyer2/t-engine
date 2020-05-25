
#ifndef CORE_COMPONENT_INTERFACE_H
#define CORE_COMPONENT_INTERFACE_H

#include "core/core.h"
#include "core/read_only_property.h"

namespace TEngine::Core {

class ComponentInterface {
public:
	Core::ReadOnlyProperty<entity> entityId;

	ComponentInterface() { } // exists so serialization works.
	ComponentInterface(entity id) : entityId(id) { }
	virtual ~ComponentInterface() { }

	virtual void instantiate(entity) = 0;
};
}
#endif
