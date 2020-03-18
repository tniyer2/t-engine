
#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include "component.h"
#include "entity.h"

namespace TEngine::Core {

// Type Unsafe base class of IComponentArray
class TU_IComponentArray {
public:
	virtual size_t getSize() = 0;
	virtual bool hasComponent(entity) = 0;
	virtual bool removeComponent(entity) = 0;
	virtual bool removeIfComponent(entity) = 0;
};

template<class T>
class IComponentArray : public TU_IComponentArray {
public:
	virtual ComponentPtr<T> getComponent(entity) = 0;
	virtual ComponentPtr<T> addComponent(entity) = 0;
};
}
#endif
