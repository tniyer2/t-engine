
#ifndef CORE_COMPONENT_PTR_H
#define CORE_COMPONENT_PTR_H

#include "component_allocator.h"
#include "entity.h"
#include <cassert>

namespace TEngine::Core {

template<class T>
class ComponentPtr {
private:
	IComponentAllocator<T>* m_allocator = nullptr;
	entity m_entity;
public:
	static ComponentPtr invalid() { return ComponentPtr(); }

	ComponentPtr() { }
	ComponentPtr(IComponentAllocator<T>& a, entity e)
		: m_allocator(&a), m_entity(e) {
		assert(m_entity != entity::invalid());
	}

	operator bool() const {
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
