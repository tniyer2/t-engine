
#ifndef GRAPHICS_TRANSFORM_H
#define GRAPHICS_TRANSFORM_H

#include "../core/component_manager.h"
#include "../core/component_ptr.h"
#include "../core/entity.h"
#include <glm/glm.hpp>

namespace TEngine::Graphics {

using Core::entity;
using Core::ComponentPtr;

class TransformComponent {
	using index_t = unsigned int;

	friend class TransformComponentArray;

	ComponentPtr<TransformComponent> getTransform(entity e) {
		auto& s_instance = Core::ComponentManager::getInstance();
		return s_instance.getComponent<TransformComponent>(e);
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

	ComponentPtr<TransformComponent> getChild(index_t);
	// adds child to the end of the list.
	void addChild(entity);
	// adds child at specified index.
	void addChild(index_t, entity);
	// removes child from list.
	void removeChild(entity);
	// removes child at specified index.
	void removeChild(index_t);
private:
	void removeChild(ComponentPtr<TransformComponent>);
};
}
#endif
