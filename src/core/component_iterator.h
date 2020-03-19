
#ifndef CORE_COMPONENT_H
#define CORE_COMPONENT_H

namespace TEngine::Core {

template<class T>
class IComponentIterator {
public:
	virtual ~IComponentIterator() { };

	virtual IComponentIterator& operator++() = 0;

	virtual operator bool() const = 0;

	virtual T* operator->() = 0;
	virtual T& operator*() = 0;
};
}
#endif
