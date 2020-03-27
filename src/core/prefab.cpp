
#include "prefab.h"
#include "core.h"

namespace TEngine::Core {

void PrefabNode::addComponent(IComponent* comp) {
	m_components.push_back(comp);
}

entity PrefabNode::instantiate(entity baseId) {
	assert(baseId);

	entity newId = this->entityId;
	newId(baseId);

	for (auto it = m_components.begin(); it != m_components.end(); ++it) {
		(*it)->instantiate(newId);
	}

	return newId;
}

PrefabNode* Prefab::createNode() {
	entity id = entity(m_nodes.size() + 1);
	PrefabNode* node = new PrefabNode(id);
	m_nodes.push_back(node);
	return node;
}

entity Prefab::instantiate() {
	if (m_nodes.size() == 0) throw "Cannot instantiate prefab. No root node.";

	entity baseId = create<entity>();
	for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
		(*it)->instantiate(baseId);
	}

	return baseId;
}
}
