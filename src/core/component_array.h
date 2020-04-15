
#ifndef CORE_COMPONENT_ARRAY_H
#define CORE_COMPONENT_ARRAY_H

#include "component_iterator.h"
#include "component_handle.h"
#include "entity.h"
#include <memory>

namespace TEngine::Core {

class BaseComponentArray {
public:
	virtual ~BaseComponentArray() { };

	virtual size_t getCount() const = 0;
	virtual bool hasComponent(entity) const = 0;
	virtual std::unique_ptr<IComponentIterator> untypedBegin() const = 0;
};

class ShrinkableComponentArray : public BaseComponentArray {
public:
	virtual void removeComponent(entity) = 0;
	virtual bool removeIfComponent(entity) = 0;
};

template<class T>
class TComponentArray : public BaseComponentArray {
public:
	virtual ComponentHandle<T> getComponent(entity) const = 0;
	virtual std::unique_ptr<TComponentIterator<T>> begin() const = 0;
};

template<class T>
class GrowableComponentArray : public TComponentArray<T> {
public:
	virtual ComponentHandle<T> addComponent(entity) = 0;
};

template<class T>
class IComponentArray :
	public GrowableComponentArray<T>,
	public ShrinkableComponentArray { };
}
#endif
