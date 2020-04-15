
#ifndef CORE_COMPONENT_HANDLE_H
#define CORE_COMPONENT_HANDLE_H

#include "component_allocator.h"
#include "entity.h"

namespace TEngine::Core {

template<class T>
class ComponentHandle {
private:
	TComponentAllocator<T>* m_allocator = nullptr;
	entity m_entity;
public:
	static ComponentHandle invalid() { return ComponentHandle(); }

	ComponentHandle() { }
	ComponentHandle(TComponentAllocator<T>& allocator, entity id)
		: m_allocator(&allocator), m_entity(id) { }

	explicit operator bool() const {
		return m_allocator && m_allocator->has(m_entity);
	}

	const T* operator->() const {
		if (m_allocator) return m_allocator->get(m_entity);
		else return nullptr;
	}
	T* operator->() {
		return const_cast<T*>(static_cast<const ComponentHandle&>(*this).operator->());
	}

	const T& operator*() const {
		const T* ptr = operator->();
		if (ptr) return *ptr;
		else throw "Invalid State. Cannot dereference component handle.";
	}
	T& operator*() {
		return const_cast<T&>(static_cast<const ComponentHandle&>(*this).operator*());
	}

	friend bool operator==(ComponentHandle a, ComponentHandle b) {
		return a.m_allocator == b.m_allocator && a.m_entity == b.m_entity;
	}
};
}
#endif
