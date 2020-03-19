
#ifndef CORE_COMPONENT_H
#define CORE_COMPONENT_H

namespace TEngine::Core {

template<class T>
class ComponentIterator {
	virtual ComponentIterator& operator++() = 0;

	virtual operator bool() const = 0;

	virtual T* operator->() = 0;
	// virtual const T* operator->() const = 0;

	virtual T& operator*() = 0;
	// virtual const T& operator*() const = 0;
};
}
#endif
