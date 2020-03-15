
#include "core.h"
#include "script.h"
#include <cassert>

namespace TEngine::Core {

ComponentManager& ComponentManager::getInstance() {
	assert(instance);
	return *instance;
}

ComponentManager::ComponentManager() {
	assert(!instance);
	instance = this;
}

EntityManager& EntityManager::getInstance() {
	assert(instance);
	return *instance;
}

EntityManager::EntityManager() {
	assert(!instance);
	instance = this;
}

void EntityManager::update(float deltaTime) {
	ComponentManager& compM = ComponentManager::getInstance();
	ScriptManager& scriptM = ScriptManager::getInstance();

	for (auto it = m_destroyMap.begin(); it != m_destroyMap.end(); ++it) {
		entity e = entity(it->first);
		scriptM.removeIfScript(e);
		compM.removeAllComponents(e);
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
