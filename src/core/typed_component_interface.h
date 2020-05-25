
#ifndef CORE_TYPED_COMPONENT_INTERFACE_H
#define CORE_TYPED_COMPONENT_INTERFACE_H

#include "component_interface.h"

namespace TEngine::Core {

template<class Derived>
class TypedComponentInterface : public ComponentInterface {
public:
	using ComponentInterface::ComponentInterface;

	void instantiate(entity id) override { _instantiate(id); }
protected:
	virtual ComponentHandle<Derived> _instantiate(entity id) {
		auto handle = create<Derived>(id);
		*handle = (Derived&)*this;
		handle->entityId = ReadOnlyProperty<entity>(id);
		return handle;
	}
};
}
#endif
