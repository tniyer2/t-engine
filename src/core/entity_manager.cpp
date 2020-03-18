
#include "entity_manager.h"
#include "component_manager.h"
#include "script_manager.h"
#include "entity.h"
#include <cassert>

namespace TEngine::Core {

void EntityManager::update(float deltaTime) {
	checkRunning();

	ComponentManager& compM = ComponentManager::getInstance();
	ScriptManager& scriptM = ScriptManager::getInstance();

	for (auto it = m_toBeDestroyed.begin(); it != m_toBeDestroyed.end(); ++it) {
		entity e = it->first;
		scriptM.removeIfScript(e);
		compM.removeComponents(e);
	}
	m_toBeDestroyed.clear();
}

entity EntityManager::create() {
	checkRunning();

	return entity(EntityManager::entityIdCounter++);
}

void EntityManager::destroy(entity e) {
	checkRunning();

	bool exists = (unsigned int)e < EntityManager::entityIdCounter;
	assert(exists);
	if (!exists) return;

	m_toBeDestroyed[e] = true;
}
}
