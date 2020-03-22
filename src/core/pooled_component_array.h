
#ifndef CORE_POOLED_COMPONENT_ARRAY_H
#define CORE_POOLED_COMPONENT_ARRAY_H

#include "pooled_component_allocator.h"
#include "component_array.h"
#include "component_ptr.h"
#include "entity.h"

namespace TEngine::Core {

template<class T>
class PooledComponentArray : public IComponentArray<T> {
	using Iterator = typename PooledComponentAllocator<T>::PooledComponentIterator;
protected:
	PooledComponentAllocator<T>& m_allocator;
public:
	PooledComponentArray(PooledComponentAllocator<T>& a) : m_allocator(a) { }

	size_t getCount() const override {
		return m_allocator.getCount();
	}

	bool hasComponent(entity e) const override {
		return m_allocator.has(e);
	}

	ComponentPtr<T> getComponent(entity e) const override {
		return ComponentPtr<T>(m_allocator, e);
	}

	std::unique_ptr<IComponentIterator<T>> begin() const override {
		return std::unique_ptr<IComponentIterator<T>>(_begin());
	}

	std::unique_ptr<TU_IComponentIterator> TUbegin() const override {
		return std::unique_ptr<TU_IComponentIterator>(_begin());
	}

	ComponentPtr<T> addComponent(entity e) override {
		bool allocated = m_allocator.allocate(e);
		assert(allocated);
		return ComponentPtr<T>(m_allocator, e);
	}

	bool removeComponent(entity e) override {
		return _removeComponent(e);
	}

	bool removeIfComponent(entity e) override {
		return m_allocator.has(e) ? _removeComponent(e) : true;
	}
protected:
	virtual bool _removeComponent(entity e) {
		bool removed = m_allocator.free(e);
		assert(removed);
		return removed;
	}
private:
	IComponentIterator<T>* _begin() const  {
		return new typename PooledComponentAllocator<T>::PooledComponentIterator(m_allocator.begin());
	}
};
}
#endif
