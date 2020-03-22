
#ifndef GRAPHICS_TRANSFORM_COMPONENT_ARRAY_H
#define GRAPHICS_TRANSFORM_COMPONENT_ARRAY_H

#include "transform_component.h"
#include "../core/entity_manager.h"
#include "../core/pooled_component_array.h"

namespace TEngine::Graphics {

using TransformArrayBase = Core::PooledComponentArray<Transform>;
using TransformPool = Core::PooledComponentAllocator<Transform>;

class TransformComponentArray : public TransformArrayBase {
private:
	ComponentPtr<Transform> m_root;
public:
	using TransformArrayBase::TransformArrayBase;

	ComponentPtr<Transform> addComponent(entity id) override {
		auto t = TransformArrayBase::addComponent(id);
		if (t) m_root->addChild(t);
		return t;
	}

	ComponentPtr<Transform> setRoot() {
		assert(!m_root);
		entity id = Core::EntityManager::getInstance().create();
		m_root = TransformArrayBase::addComponent(id);
		return m_root;
	}
private:
	bool _removeComponent(entity id) override {
		auto t = getComponent(id);
		if (t) {
			if (t->m_parent) {
				Transform::get(t->m_parent)->removeChild(t);
			}
			t->removeChildren();
		}
		return TransformArrayBase::_removeComponent(id);
	}
};
}
#endif
