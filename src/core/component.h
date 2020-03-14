
#ifndef CORE_COMPONENT_H
#define CORE_COMPONENT_H

#include "memory.h"
#include "id.h"
#include <cassert>

namespace TEngine::Core {

template<class T>
class IComponentAllocator : public IDerivedAllocator {
public:
	IComponentAllocator(IRootAllocator& a) : IDerivedAllocator(a) { }

	virtual bool allocate(entity) = 0;
	virtual bool has(entity) = 0;
	virtual T* get(entity) = 0;
	virtual bool free(entity) = 0;
};

template<class T>
class ComponentPtr {
private:
	IComponentAllocator<T>& m_allocator;
	entity m_entity;
public:
	ComponentPtr(IComponentAllocator<T>& a, entity e) : m_allocator(a), m_entity(e) {
		assert(m_entity != entity::invalid());
	}

	T* operator->() {
		return m_allocator.get(m_entity);
	}
	const T* operator->() const {
		return m_allocator.get(m_entity);
	}

	T& operator*() {
		T* ptr = m_allocator.get(m_entity);
		assert(ptr);
		return *ptr;
	}
	const T& operator*() const {
		T* ptr = m_allocator.get(m_entity);
		assert(ptr);
		return *ptr;
	}
};
}
#endif
