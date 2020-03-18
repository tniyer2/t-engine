
#ifndef CORE_POOLED_COMPONENT_ARRAY_H
#define CORE_POOLED_COMPONENT_ARRAY_H

#include "component_pool.h"
#include "component_array.h"
#include "component.h"
#include "entity.h"
#include "../utility/boolean.h"
#include <variant>

namespace TEngine::Core {

template<class T>
class PooledComponentArray : public IComponentArray<T> {
protected:
	PooledComponentAllocator<T>& m_allocator;
public:
	PooledComponentArray(PooledComponentAllocator<T>& a) : m_allocator(a) { }

	size_t getSize() override {
		return m_allocator.getCount();
	}

	bool hasComponent(entity e) override {
		return m_allocator.has(e);
	}

	ComponentPtr<T> getComponent(entity e) override {
		return ComponentPtr<T>(m_allocator, e);
	}

	ComponentPtr<T> addComponent(entity e) override {
		bool allocated = m_allocator.allocate(e);
		assert(allocated);
		return ComponentPtr<T>(m_allocator, e);
	}

	bool removeComponent(entity e) override {
		bool freed = m_allocator.free(e);
		assert(freed);
		return freed;
	}

	bool removeIfComponent(entity e) override {
		return m_allocator.has(e) ? removeComponent(e) : true;
	}
};
}
#endif
