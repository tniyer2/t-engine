
#ifndef CORE_COMPONENT_ITERATOR_H
#define CORE_COMPONENT_ITERATOR_H

#include "entity.h"

namespace TEngine::Core {

class TU_IComponentIterator {
public:
	virtual operator bool() const = 0;
	virtual TU_IComponentIterator& operator++() = 0;
	virtual entity getEntity() = 0;
};

template<class T>
class IComponentIterator : public TU_IComponentIterator {
public:
	virtual ~IComponentIterator() { };
	virtual T* operator->() = 0;
	virtual T& operator*() = 0;
};
}
#endif
