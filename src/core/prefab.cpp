
#include "prefab.h"
#include "core.h"

namespace TEngine::Core {

entity PrefabNode::instantiate(ComponentHandle<Transform> parent) {
	entity id = create<entity>();
	
	auto transform = create<Transform>(id);
	transform->setLocalMatrix(matrix);
	if (parent) parent->addChild(transform);

	for (auto it = components.begin(); it != components.end(); ++it) {
		(*it)->instantiate(id);
	}

	for (auto it = children.begin(); it != children.end(); ++it) {
		(*it)->instantiate(transform);
	}

	return id;
}

entity Prefab::instantiate() {
	return m_rootNode->instantiate(Transform::getRoot());
}
}
