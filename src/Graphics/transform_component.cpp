
#include "transform_component.h"
#include <cassert>

namespace TEngine::Graphics {

TransformPtr TransformComponent::getChild(entity e) const {
	auto child = Core::ComponentManager::getInstance().getComponent<Transform>(e);
	bool isChild = entityId == child->m_parent;
	return isChild ? child : TransformPtr::invalid();
}

template<bool forwards>
TransformPtr TransformComponent::getChildTemplate(index_t index) const {
	entity e = forwards ? m_firstChild : m_lastChild;
	index_t count = forwards ? 0 : m_numChildren - 1;

	while ((bool)e) {
		assert(count < m_numChildren);

		const auto transform = getChild(e);
		if (index == count) return transform;

		e = forwards ? transform->m_nextSibling : transform->m_prevSibling;
		forwards ? ++count : --count;
	}

	return TransformPtr::invalid();
}

template TransformPtr TransformComponent::getChildTemplate<true>(index_t) const;
template TransformPtr TransformComponent::getChildTemplate<false>(index_t) const;

TransformPtr TransformComponent::getChild(index_t index) const {
	if (index >= m_numChildren) return TransformPtr::invalid();
	return index <= m_numChildren / 2 ?
		getChildTemplate<true>(index) :
		getChildTemplate<false>(index);
}

void TransformComponent::addChild(entity e) {
	addChild(m_numChildren, e);
}

void TransformComponent::addChild(index_t index, entity e) {
	bool validIndex = index <= m_numChildren;
	assert(validIndex);
	if (!validIndex) return;

	auto newChild = getChild(e);

	// remove from existing parent
	if (newChild->m_parent != entity::invalid()) {
		getChild(newChild->m_parent)->removeChild(e);
	}

	bool first = index == 0;
	bool last = index == m_numChildren;
	TransformPtr prev;
	TransformPtr next;

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
	m_numChildren++;
}

void TransformComponent::removeChild(entity e) {
	_removeChild(getChild(e));
}

void TransformComponent::removeChild(index_t index) {
	_removeChild(getChild(index));
}

void TransformComponent::_removeChild(TransformPtr child) {
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
		auto prev = getChild(child->m_prevSibling);
		prev->m_nextSibling = child->m_nextSibling;
	}

	if (last) {
		m_lastChild = child->m_prevSibling;
	}
	else {
		auto next = getChild(child->m_nextSibling);
		next->m_prevSibling = child->m_prevSibling;
	}

	child->m_prevSibling = entity::invalid();
	child->m_nextSibling = entity::invalid();
	child->m_parent = entity::invalid();
	--m_numChildren;
}
}
