
#ifndef CORE_COMPONENT_H
#define CORE_COMPONENT_H

#include "core/core.h"
#include "core/read_only_property.h"

namespace TEngine::Core {

class IComponent {
public:
	Core::ReadOnlyProperty<entity> entityId;

	IComponent() { } // exists so serialization works.
	IComponent(entity id) : entityId(id) { }
	virtual ~IComponent() { }

	virtual void instantiate(entity) = 0;
};

template<class Derived>
class TComponent : public IComponent {
public:
	using IComponent::IComponent;

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
