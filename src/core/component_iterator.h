
#ifndef CORE_COMPONENT_ITERATOR_H
#define CORE_COMPONENT_ITERATOR_H

#include "entity.h"

namespace TEngine::Core {

class UntypedComponentIterator {
public:
	virtual ~UntypedComponentIterator() { }

	virtual explicit operator bool() const = 0;
	virtual entity getEntity() const = 0;

	virtual UntypedComponentIterator& operator++() = 0;
};

template<class T>
class IComponentIterator : public UntypedComponentIterator {
public:
	virtual const T* operator->() const = 0;
	virtual T* operator->() = 0;
	virtual const T& operator*() const = 0;
	virtual T& operator*() = 0;
};
}
#endif
