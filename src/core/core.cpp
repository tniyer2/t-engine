
#include "core.h"

namespace TEngine::Core {

void EntityManager::update(float deltaTime) {
	ComponentManager& instance = ComponentManager::getInstance();
	for (auto it = m_destroyMap.begin(); it != m_destroyMap.end(); ++it) {
		instance.removeAllComponents(entity(it->first));
		m_destroyMap.erase(it);
	}
}

bool EntityManager::destroy(entity e) {
	auto id = (unsigned int)e;
	bool exists = id < EntityManager::entityIdCounter;
	assert(exists);
	if (!exists) return false;

	m_destroyMap[id] = true;
	return true;
}
}
