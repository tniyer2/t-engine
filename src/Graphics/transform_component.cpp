
#include "transform_component.h"

namespace TEngine::Graphics {

ComponentPtr<TransformComponent> TransformComponent::getChild(index_t index) {
	bool validIndex = index < m_numChildren;
	assert(validIndex);
	if (!validIndex) return ComponentPtr<TransformComponent>();

	bool forward = index <= m_numChildren / 2;

	entity e = forward ? m_firstChild : m_lastChild;
	size_t count = forward ? 0 : m_numChildren - 1;

	while (e != entity::invalid()) {
		auto transform = getTransform(e);
		if (index == count) {
			return transform;
		}
		e = forward ? transform->m_nextSibling : transform->m_prevSibling;
		forward ? count++ : count--;
	}

	return ComponentPtr<TransformComponent>();
}

void TransformComponent::addChild(entity e) {
	addChild(m_numChildren, e);
}

void TransformComponent::addChild(index_t index, entity e) {
	bool validIndex = index <= m_numChildren;
	assert(validIndex);
	if (!validIndex) return;

	auto newChild = getTransform(e);

	// remove from existing parent
	if (newChild->m_parent != entity::invalid()) {
		getTransform(newChild->m_parent)->removeChild(e);
	}

	bool first = index == 0;
	bool last = index == m_numChildren;
	ComponentPtr<TransformComponent> prev;
	ComponentPtr<TransformComponent> next;

	if (!first) prev = getChild(index - 1);
	if (!last) next = getChild(index);

	if (first) {
		m_firstChild = e;
		newChild->m_prevSibling = entity::invalid();
	}
	else {
		prev->m_nextSibling = e;
		newChild->m_prevSibling = prev->m_self;
	}

	if (last) {
		m_lastChild = e;
		newChild->m_nextSibling = entity::invalid();
	}
	else {
		next->m_prevSibling = e;
		newChild->m_nextSibling = next->m_self;
	}

	newChild->m_parent = m_self;
	m_numChildren++;
}

void TransformComponent::removeChild(entity e) {
	removeChild(getTransform(e));
}

void TransformComponent::removeChild(index_t index) {
	removeChild(getChild(index));
}

void TransformComponent::removeChild(ComponentPtr<TransformComponent> child) {
	assert(child);
	if (!child) return;

	bool myChild = child->m_parent == m_self;
	assert(myChild);
	if (!myChild) return;

	bool first = !child->m_prevSibling;
	bool last = !child->m_nextSibling;

	if (first) {
		m_firstChild = child->m_nextSibling;
	}
	else {
		auto prev = getTransform(child->m_prevSibling);
		prev->m_nextSibling = child->m_nextSibling;
	}

	if (last) {
		m_lastChild = child->m_prevSibling;
	}
	else {
		auto next = getTransform(child->m_nextSibling);
		next->m_prevSibling = child->m_prevSibling;
	}

	child->m_prevSibling = entity::invalid();
	child->m_nextSibling = entity::invalid();
	child->m_parent = entity::invalid();
	m_numChildren--;
}
}
