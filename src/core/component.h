
#ifndef CORE_COMPONENT_H
#define CORE_COMPONENT_H

#include "core/core.h"
#include "core/read_only_property.h"

namespace TEngine::Core {

class IComponent {
public:
	Core::ReadOnlyProperty<entity> entityId;

	IComponent() { }
	IComponent(entity id) : entityId(id) { }
	virtual ~IComponent() { }

	virtual entity instantiate(entity) = 0;
};

template<class T>
class TComponent : public IComponent {
public:
	using IComponent::IComponent;

	entity instantiate(entity newId) override {
		_instantiate(newId);
		return newId;
	}
protected:
	virtual ComponentHandle<T> _instantiate(entity newId) {
		auto handle = create<T>(newId);
		*handle = (T&)*this;
		handle->entityId = ReadOnlyProperty<entity>(newId);
		return handle;
	}
};
}
#endif
