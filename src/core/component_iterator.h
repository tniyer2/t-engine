
#ifndef CORE_COMPONENT_ITERATOR_H
#define CORE_COMPONENT_ITERATOR_H

namespace TEngine::Core {

class IComponentIterator {
public:
	virtual ~IComponentIterator() { }

	virtual explicit operator bool() const = 0;
	virtual bool operator==(const IComponentIterator&) const = 0;

	virtual IComponentIterator& operator++() = 0;
};

template<class T>
class TComponentIterator : public IComponentIterator {
public:
	virtual const T* operator->() const = 0;
	virtual const T& operator*() const = 0;

	virtual T* operator->() = 0;
	virtual T& operator*() = 0;
};
}
#endif
