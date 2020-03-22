
#ifndef GRAPHICS_SORTED_TRANSFORM_ITERATOR_H
#define GRAPHICS_SORTED_TRANSFORM_ITERATOR_H

#include "transform_component.h"
#include "../core/component_iterator.h"

namespace TEngine::Graphics {

class SortedTransformIterator : public Core::IComponentIterator<Transform> {
private:
	ComponentPtr<Transform> m_prev;
	ComponentPtr<Transform> m_cur;
public:
	SortedTransformIterator(ComponentPtr<Transform> root) : m_cur(root) {
		m_cur->m_computed = m_cur->m_matrix;
	}

	TU_IComponentIterator& operator++() override {
		increment<true>();
		return *this;
	}

	explicit operator bool() const override { return (bool)m_cur; }
	entity getEntity() override { return m_cur->entityId; }
	Transform* operator->() override { return &*m_cur; }
	Transform& operator*() override { return *m_cur; }
private:
	template<bool iterateDown>
	void increment() {
		if (iterateDown && m_cur->m_firstChild) {
			m_prev = m_cur;
			m_cur = Transform::get(m_cur->m_firstChild);
			m_cur->m_computed = m_prev->m_computed * m_cur->m_matrix;
		}
		else if (m_cur->m_nextSibling) {
			m_cur = Transform::get(m_cur->m_nextSibling);
			m_cur->m_computed = m_prev->m_computed * m_cur->m_matrix;
		}
		else if (!m_prev) {
			m_cur = ComponentPtr<Transform>::invalid();
		}
		else {
			m_cur = m_prev;
			m_prev = Transform::get(m_cur->m_parent);
			increment<false>();
		}
	}
};
}
#endif
