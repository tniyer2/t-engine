
#ifndef GRAPHICS_TRANSFORM_H
#define GRAPHICS_TRANSFORM_H

#include "../core/component_manager.h"
#include "../core/pooled_component_array.h"
#include "../core/component.h"
#include "../core/entity.h"
#include <glm/glm.hpp>

namespace TEngine::Graphics {

using Core::entity;
using Core::ComponentPtr;

class Transform {
	using index_t = unsigned int;

	friend class TransformComponentArray;

	ComponentPtr<Transform> getTransform(entity e) {
		auto& s_instance = Core::ComponentManager::getInstance();
		return s_instance.getComponent<Transform>(e);
	}
public:
	glm::mat4 matrix;
	glm::mat4 computed;
private:
	size_t m_numChildren = 0;
	entity m_self;
	entity m_parent;
	entity m_firstChild;
	entity m_lastChild;
	entity m_prevSibling;
	entity m_nextSibling;
public:
	size_t getNumChildren() { return m_numChildren; }

	ComponentPtr<Transform> getChild(index_t);
	// adds child to the end of the list.
	void addChild(entity);
	// adds child at specified index.
	void addChild(index_t, entity);
	// removes child from list.
	void removeChild(entity);
	// removes child at specified index.
	void removeChild(index_t);
private:
	void removeChild(ComponentPtr<Transform>);
};

class TransformComponentArray : public Core::PooledComponentArray<Transform> {
public:
	using Core::PooledComponentArray<Transform>::PooledComponentArray;

	virtual ComponentPtr<Transform> addComponent(entity e) {
		bool allocated = m_allocator.allocate(e);
		assert(allocated);
		ComponentPtr<Transform> ptr(m_allocator, e);
		ptr->m_self = e;
		return ptr;
	}
};
}
#endif
