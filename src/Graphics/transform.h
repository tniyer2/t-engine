
#ifndef GRAPHICS_TRANSFORM_COMPONENT_H
#define GRAPHICS_TRANSFORM_COMPONENT_H

#include "core/component.h"
#include "core/component_handle.h"
#include "core/entity.h"
#include <glm/glm.hpp>

namespace TEngine::Graphics {

using Core::ComponentHandle;
using Core::entity;

class Transform : public Core::TComponent<Transform> {
	using index_t = unsigned int;

	friend class TransformComponentArray;
	friend class DepthFirstTransformIterator;
private:
	inline static ComponentHandle<Transform> m_rootTransform;

	glm::mat4 m_matrix;
	glm::mat4 m_computed;

	entity m_parent;
	entity m_prevSibling;
	entity m_nextSibling;
	entity m_firstChild;
	entity m_lastChild;

	index_t m_childCount = 0;
	bool m_dirty = false;
public:
	static ComponentHandle<Transform> getRoot() { return m_rootTransform; }

	using TComponent<Transform>::TComponent;

	entity instantiate(entity) override;

	glm::mat4 getLocalMatrix() const { return m_matrix; }
	glm::mat4 getWorldMatrix() const { return m_computed; }

	index_t getChildCount() const { return m_childCount; }
	ComponentHandle<Transform> getParent() const;
	ComponentHandle<Transform> getChild(index_t) const;

	void setLocalMatrix(glm::mat4);

	void addChild(ComponentHandle<Transform>); // adds to end.
	void addChild(index_t, ComponentHandle<Transform>);

	void addChild(entity); // adds to end.
	void addChild(index_t, entity);

	void removeChild(ComponentHandle<Transform>);
	void removeChild(entity);
	void removeChild(index_t);

	void removeChildren();

	void removeFromParent();
private:
	template<bool>
	ComponentHandle<Transform> _getChild(index_t) const;

	void _addChild(index_t, ComponentHandle<Transform>);
	void _removeChild(ComponentHandle<Transform>);
};
}
#endif
