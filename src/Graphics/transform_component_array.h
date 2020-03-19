
#ifndef GRAPHICS_TRANSFORM_COMPONENT_ARRAY_H
#define GRAPHICS_TRANSFORM_COMPONENT_ARRAY_H

#include "transform_component.h"
#include "../core/pooled_component_array.h"
#include "../core/component_ptr.h"
#include "../core/entity.h"
#include <cassert>

namespace TEngine::Graphics {

using Core::entity;
using Core::ComponentPtr;

class TransformComponentArray : public Core::PooledComponentArray<TransformComponent> {
public:
	using Core::PooledComponentArray<TransformComponent>::PooledComponentArray;

	ComponentPtr<TransformComponent> addComponent(entity e) override {
		bool allocated = m_allocator.allocate(e);
		assert(allocated);
		ComponentPtr<TransformComponent> ptr(m_allocator, e);
		ptr->m_self = e;
		return ptr;
	}
};
}
#endif
