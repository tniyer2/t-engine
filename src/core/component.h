
#ifndef CORE_COMPONENT_H
#define CORE_COMPONENT_H

#include "memory.h"
#include "id.h"
#include <cassert>

namespace TEngine::Core {

template<class T>
class IComponentAllocator : public IDerivedAllocator {
protected:
	size_t m_count = 0;
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
	IComponentAllocator<T>* m_allocator = nullptr;
	entity m_entity;
public:
	ComponentPtr() { }

	ComponentPtr(IComponentAllocator<T>& a, entity e)
		: m_allocator(&a), m_entity(e) {
		assert(m_entity != entity::invalid());
	}

	operator bool() {
		return m_allocator && m_allocator->has(m_entity);
	}

	T* operator->() {
		assert(m_allocator);
		return m_allocator->get(m_entity);
	}
	const T* operator->() const {
		assert(m_allocator);
		return m_allocator->get(m_entity);
	}

	T& operator*() {
		assert(m_allocator);
		T* ptr = m_allocator->get(m_entity);
		assert(ptr);
		return *ptr;
	}
	const T& operator*() const {
		assert(m_allocator);
		T* ptr = m_allocator->get(m_entity);
		assert(ptr);
		return *ptr;
	}
};
}
#endif
