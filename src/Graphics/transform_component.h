
#ifndef GRAPHICS_TRANSFORM_COMPONENT_H
#define GRAPHICS_TRANSFORM_COMPONENT_H

#include "../core/component_manager.h"
#include "../core/component_ptr.h"
#include "../core/component.h"
#include "../core/entity.h"
#include <glm/glm.hpp>

namespace TEngine::Graphics {

using Core::entity;
using Core::ComponentPtr;

class TransformComponent : public Core::IComponent {
	using index_t = unsigned int;
public:
	glm::mat4 matrix;
	glm::mat4 computed;
	const entity entityId;
private:
	index_t m_numChildren = 0;
	entity m_parent;
	entity m_firstChild;
	entity m_lastChild;
	entity m_prevSibling;
	entity m_nextSibling;
public:
	TransformComponent(entity e) : entityId(e) { }

	index_t getNumChildren() const { return m_numChildren; }
	ComponentPtr<TransformComponent> getChild(index_t) const;

	void addChild(entity); // adds child to the end of the list.
	void addChild(index_t, entity); // adds child at specified index.
	void removeChild(entity); // removes child from list.
	void removeChild(index_t); // removes child at specified index.
private:
	static ComponentPtr<TransformComponent> getTransform(entity);

	void _removeChild(ComponentPtr<TransformComponent>);
};
}
#endif
