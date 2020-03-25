
#ifndef CORE_COMPONENT_ARRAY_H
#define CORE_COMPONENT_ARRAY_H

#include "component_iterator.h"
#include "component_handle.h"
#include "entity.h"
#include <memory>

namespace TEngine::Core {

class UntypedComponentArray {
public:
	virtual ~UntypedComponentArray() { };

	virtual size_t getCount() const = 0;
	virtual bool hasComponent(entity) const = 0;

	virtual std::unique_ptr<UntypedComponentIterator> untypedBegin() = 0;
	virtual void removeComponent(entity) = 0;
	virtual bool removeIfComponent(entity) = 0;
};

template<class T>
class IComponentArray : public UntypedComponentArray {
public:
	virtual ComponentHandle<T> getComponent(entity) const = 0;

	virtual std::unique_ptr<IComponentIterator<T>> begin() = 0;
	virtual ComponentHandle<T> addComponent(entity) = 0;
};
}
#endif
