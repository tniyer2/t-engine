
#ifndef GRAPHICS_DEPTH_FIRST_TRANSFORM_ITERATOR_H
#define GRAPHICS_DEPTH_FIRST_TRANSFORM_ITERATOR_H

#include "transform.h"
#include "core/component_iterator.h"
#include "core/core.h"
#include <stdexcept>

namespace TEngine::Graphics {

class DepthFirstTransformIterator : public Core::IComponentIterator<Transform> {
private:
	constexpr static const char* FINISHED = "Invalid Call. Iterator is finished.";

	const ComponentHandle<Transform> m_start;
	ComponentHandle<Transform> m_prev, m_cur;
public:
	DepthFirstTransformIterator(ComponentHandle<Transform> start)
		: m_start(start), m_cur(start) { }

	UntypedComponentIterator& operator++() override {
		increment<true>();
		return *this;
	}

	explicit operator bool() const override { return (bool)m_cur; }

	const Transform* operator->() const override {
		if (!m_cur) throw new std::out_of_range(FINISHED);
		return &*m_cur;
	}
	Transform* operator->() override {
		return const_cast<Transform*>(static_cast<const DepthFirstTransformIterator&>(*this).operator->());
	}

	const Transform& operator*() const override {
		if (!m_cur) throw new std::out_of_range(FINISHED);
		return *m_cur;
	}
	Transform& operator*() override {
		return const_cast<Transform&>(static_cast<const DepthFirstTransformIterator&>(*this).operator*());
	}
private:
	template<bool iterateDown>
	void increment() {
		if (!m_cur) throw "Invalid State. Cannot dereference Transform.";

		if (iterateDown && m_cur->m_firstChild) {
			m_prev = m_cur;
			m_cur = Core::get<Transform>(m_cur->m_firstChild);
			computeMatrix();
		}
		else if (m_cur->m_nextSibling) {
			m_cur->m_dirty = false;
			m_cur = Core::get<Transform>(m_cur->m_nextSibling);
			computeMatrix();
		}
		else if (m_cur == m_start) {
			m_cur->m_dirty = false;
			m_cur = ComponentHandle<Transform>::invalid();
		}
		else {
			m_cur->m_dirty = false;
			m_cur = m_prev;
			m_prev = Core::get<Transform>(m_cur->m_parent);
			increment<false>();
		}
	}

	void computeMatrix() {
		if (m_prev->m_dirty || m_cur->m_dirty) {
			m_cur->m_dirty = true;
			m_cur->m_computed = m_prev->m_computed * m_cur->m_matrix;
		}
	}
};
}
#endif
