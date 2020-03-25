
#include "transform.h"

#include "core/core.h"
#include "utility/if_debug.h"
#include <cassert>

namespace TEngine::Graphics {

void Transform::setLocalMatrix(glm::mat4 matrix) {
	m_matrix = matrix;
	m_dirty = true;
}

ComponentHandle<Transform> Transform::getParent() const {
	return Core::get<Transform>(m_parent);
}

ComponentHandle<Transform> Transform::getChild(index_t index) const {
	if (index >= m_childCount) {
		return ComponentHandle<Transform>::invalid();
	}
	else if (index < (m_childCount / 2)) {
		return _getChild<true>(index);
	}
	else {
		return _getChild<false>(index);
	}
}

template<bool forwards>
ComponentHandle<Transform> Transform::_getChild(index_t index) const {
	entity id = forwards ? m_firstChild : m_lastChild;
	index_t count = forwards ? 0 : m_childCount - 1;

	while (id) {
		assert(count < m_childCount);

		auto child = Core::get<Transform>(id);
		assert(child->m_parent == entityId);
		if (index == count) return child;

		id = forwards ? child->m_nextSibling : child->m_prevSibling;
		forwards ? ++count : --count;
	}
	assert(count == (forwards ? m_childCount : (index_t)-1));
	assert(false);

	return ComponentHandle<Transform>::invalid();
}

template ComponentHandle<Transform> Transform::_getChild<true>(index_t) const;
template ComponentHandle<Transform> Transform::_getChild<false>(index_t) const;

void Transform::addChild(ComponentHandle<Transform> handle) {
	_addChild(m_childCount, handle);
}

void Transform::addChild(index_t index, ComponentHandle<Transform> handle) {
	_addChild(index, handle);
}

void Transform::addChild(entity id) {
	_addChild(m_childCount, Core::get<Transform>(id));
}

void Transform::addChild(index_t index, entity id) {
	_addChild(index, Core::get<Transform>(id));
}

void Transform::removeChild(ComponentHandle<Transform> handle) {
	_removeChild(handle);
}

void Transform::removeChild(entity id) {
	_removeChild(Core::get<Transform>(id));
}

void Transform::removeChild(index_t index) {
	_removeChild(getChild(index));
}

void Transform::removeChildren() {
	entity id = m_firstChild;
	index_t count = 0;

	while (id) {
		assert(count < m_childCount);

		auto child = Core::get<Transform>(id);
		assert(child->m_parent == entityId);
		id = child->m_nextSibling;

		child->m_parent = entity::invalid();
		child->m_prevSibling = entity::invalid();
		child->m_nextSibling = entity::invalid();
		child->m_dirty = true;

		IF_DEBUG(++count;)
	}
	assert(count == m_childCount);
	UNUSED(count);

	m_firstChild = entity::invalid();
	m_lastChild = entity::invalid();
	m_childCount = 0;
}

void Transform::removeFromParent() {
	auto parent = Core::get<Transform>(m_parent);
	if (parent) parent->removeChild(this->entityId);
}

constexpr static const char* InvalidHandle = "Invalid Argument. Invalid handle to transform.";

void Transform::_addChild(index_t index, ComponentHandle<Transform> newChild) {
	if (!newChild) throw InvalidHandle;
	if (index > m_childCount) throw "Invalid Argument. Invalid index to add child at.";

	newChild->removeFromParent();

	bool first = index == 0;
	bool last = index == m_childCount;
	ComponentHandle<Transform> prev, next;

	if (!first) {
		prev = getChild(index - 1);
		assert(prev);
	}
	if (!last) {
		next = getChild(index);
		assert(next);
	}

	entity newId = newChild->entityId;

	if (first) {
		m_firstChild = newId;
	}
	else {
		prev->m_nextSibling = newId;
		newChild->m_prevSibling = prev->entityId;
	}

	if (last) {
		m_lastChild = newId;
	}
	else {
		next->m_prevSibling = newId;
		newChild->m_nextSibling = next->entityId;
	}

	newChild->m_parent = this->entityId;
	newChild->m_dirty = true;
	m_childCount++;
}

void Transform::_removeChild(ComponentHandle<Transform> child) {
	if (!child) throw InvalidHandle;
	if (child->m_parent != this->entityId) throw "Invalid Argument. Transform is not a child.";

	bool first = !child->m_prevSibling;
	bool last = !child->m_nextSibling;

	if (first) {
		m_firstChild = child->m_nextSibling;
	}
	else {
		auto prev = Core::get<Transform>(child->m_prevSibling);
		assert(prev->m_parent == this->entityId);
		prev->m_nextSibling = child->m_nextSibling;
	}

	if (last) {
		m_lastChild = child->m_prevSibling;
	}
	else {
		auto next = Core::get<Transform>(child->m_nextSibling);
		assert(next->m_parent == this->entityId);
		next->m_prevSibling = child->m_prevSibling;
	}

	child->m_parent = entity::invalid();
	child->m_prevSibling = entity::invalid();
	child->m_nextSibling = entity::invalid();
	child->m_dirty = true;
	--m_childCount;
}
}
