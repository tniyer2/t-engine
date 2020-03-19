
#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include "component_iterator.h"
#include "component_ptr.h"
#include "entity.h"
#include <memory>

namespace TEngine::Core {

// Type Unsafe base class of IComponentArray
class TU_IComponentArray {
public:
	virtual ~TU_IComponentArray() { };
	virtual size_t getCount() const = 0;
	virtual bool hasComponent(entity) const = 0;
	virtual bool removeComponent(entity) = 0;
	virtual bool removeIfComponent(entity) = 0;
};

template<class T>
class IComponentArray : public TU_IComponentArray {
public:
	virtual ComponentPtr<T> getComponent(entity) const = 0;
	virtual ComponentPtr<T> addComponent(entity) = 0;
	virtual std::shared_ptr<IComponentIterator<T>> begin() const = 0;
};
}
#endif
