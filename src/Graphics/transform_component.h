
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

class Transform : public Core::IComponent {
	using index_t = unsigned int;
	friend class TransformComponentArray;
	friend class SortedTransformIterator;
private:
	glm::mat4 m_matrix;
	glm::mat4 m_computed;
public:
	const entity entityId;
private:
	entity m_parent;
	entity m_prevSibling;
	entity m_nextSibling;
	entity m_firstChild;
	entity m_lastChild;
	index_t m_numChildren = 0;
	// bool m_dirty = true;
public:
	Transform(entity id) : entityId(id) { }

	void setMatrix(glm::mat4 matrix) {
		m_matrix = matrix;
		// m_dirty = true;
	}

	index_t getNumChildren() const { return m_numChildren; }
	glm::mat4 getMatrix() const { return m_matrix; }
	glm::mat4 getWorldMatrix() const { return m_computed; }

	ComponentPtr<Transform> getChildTemplate(index_t, bool) const;

	ComponentPtr<Transform> getChild(index_t) const;

	void addChild(entity); // adds child to the end.
	void addChild(index_t, entity); // adds child at index.
	void addChild(ComponentPtr<Transform>); // adds child to the end.
	void addChild(index_t, ComponentPtr<Transform>); // adds child at index.

	void removeChild(entity); // removes child.
	void removeChild(ComponentPtr<Transform>); // removes child.
	void removeChild(index_t); // removes child at index.
	void removeChildren(); // removes all children.
private:
	static ComponentPtr<Transform> get(entity id) {
		return Core::ComponentManager::getInstance().getComponent<Transform>(id);
	}

	void _addChild(index_t, entity);
	void _removeChild(ComponentPtr<Transform>);
};
}
#endif
