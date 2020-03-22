
#include "transform_component.h"
#include <cassert>

namespace TEngine::Graphics {

ComponentPtr<Transform> Transform::getChildTemplate(index_t index, bool forwards) const {
	entity e = forwards ? m_firstChild : m_lastChild;
	index_t count = forwards ? 0 : m_numChildren - 1;

	while (e) {
		assert(count < m_numChildren);

		const auto transform = Transform::get(e);
		if (index == count) return transform;

		e = forwards ? transform->m_nextSibling : transform->m_prevSibling;
		forwards ? ++count : --count;
	}

	return ComponentPtr<Transform>::invalid();
}

ComponentPtr<Transform> Transform::getChild(index_t index) const {
	if (index >= m_numChildren) return ComponentPtr<Transform>::invalid();
	return index <= m_numChildren / 2 ?
		getChildTemplate(index, true) :
		getChildTemplate(index, false);
}

void Transform::addChild(entity e) {
	_addChild(m_numChildren, e);
}

void Transform::addChild(index_t index, entity e) {
	_addChild(index, e);
}

void Transform::addChild(ComponentPtr<Transform> ptr) {
	_addChild(m_numChildren, ptr->entityId);
}

void Transform::addChild(index_t index, ComponentPtr<Transform> ptr) {
	_addChild(index, ptr->entityId);
}

void Transform::removeChild(entity e) {
	_removeChild(Transform::get(e));
}

void Transform::removeChild(ComponentPtr<Transform> ptr) {
	_removeChild(ptr);
}

void Transform::removeChild(index_t index) {
	_removeChild(getChild(index));
}

void Transform::removeChildren() {
	entity id = m_firstChild;
	while (id) {
		auto child = Transform::get(id);
		id = child->m_nextSibling;

		child->m_parent = entity::invalid();
		child->m_prevSibling = entity::invalid();
		child->m_nextSibling = entity::invalid();
		// child->m_dirty = true;
	}
	m_firstChild = entity::invalid();
	m_lastChild = entity::invalid();
}

void Transform::_addChild(index_t index, entity e) {
	bool validIndex = index <= m_numChildren;
	assert(validIndex);
	if (!validIndex) return;

	auto newChild = Transform::get(e);

	// remove from existing parent
	if (newChild->m_parent != entity::invalid()) {
		Transform::get(newChild->m_parent)->removeChild(e);
	}

	bool first = index == 0;
	bool last = index == m_numChildren;
	ComponentPtr<Transform> prev;
	ComponentPtr<Transform> next;

	if (!first) prev = getChild(index - 1);
	if (!last) next = getChild(index);

	if (first) {
		m_firstChild = e;
		newChild->m_prevSibling = entity::invalid();
	}
	else {
		prev->m_nextSibling = e;
		newChild->m_prevSibling = prev->entityId;
	}

	if (last) {
		m_lastChild = e;
		newChild->m_nextSibling = entity::invalid();
	}
	else {
		next->m_prevSibling = e;
		newChild->m_nextSibling = next->entityId;
	}

	newChild->m_parent = entityId;
	// newChild->m_dirty = true;
	m_numChildren++;
}

void Transform::_removeChild(ComponentPtr<Transform> child) {
	assert(child);
	if (!child) return;
	bool isChild = entityId == child->m_parent;
	assert(isChild);
	if (!isChild) return;

	bool first = !child->m_prevSibling;
	bool last = !child->m_nextSibling;

	if (first) {
		m_firstChild = child->m_nextSibling;
	}
	else {
		auto prev = Transform::get(child->m_prevSibling);
		prev->m_nextSibling = child->m_nextSibling;
	}

	if (last) {
		m_lastChild = child->m_prevSibling;
	}
	else {
		auto next = Transform::get(child->m_nextSibling);
		next->m_prevSibling = child->m_prevSibling;
	}

	child->m_prevSibling = entity::invalid();
	child->m_nextSibling = entity::invalid();
	child->m_parent = entity::invalid();
	// child->m_dirty = true;
	--m_numChildren;
}
}
