
#include "transform_component_array.h"

#include "core/core.h"
#include <cassert>

namespace TEngine::Graphics {

void TransformComponentArray::setRootTransform() {
	if (Transform::m_rootTransform) throw "Invalid Call. Transform root is already set.";
	entity id = Core::create<entity>();
	Transform::m_rootTransform = addComponent(id);
	assert(Transform::m_rootTransform);
}

void TransformComponentArray::_removeComponent(entity id) {
	auto transform = getComponent(id);
	if (!transform) throw "Invalid Argument. Transform component with id does not exist.";
	transform->removeFromParent();
	transform->removeChildren();
	return Core::PooledComponentArray<Transform>::_removeComponent(id);
}
}
