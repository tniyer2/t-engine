
#include "entity_manager.h"

#include "component_manager.h"
#include "script_manager.h"
#include <cassert>

namespace TEngine::Core {

void EntityManager::update(float deltaTime) {
	checkRunning();

	ComponentManager& compM = ComponentManager::getInstance();
	ScriptManager& scriptM = ScriptManager::getInstance();

	for (auto it = m_toBeDestroyed.begin(); it != m_toBeDestroyed.end(); ++it) {
		entity id = it->first;
		scriptM.removeIfScript(id);
		compM.removeComponents(id);
	}
	m_toBeDestroyed.clear();
}

entity EntityManager::create() {
	checkRunning();
	return entity(EntityManager::entityIdCounter++);
}

void EntityManager::destroy(entity id) {
	checkRunning();
	if ((unsigned int)id >= (unsigned int)EntityManager::entityIdCounter) {
		throw "Invalid Argument.";
	}
	m_toBeDestroyed[id] = true;
}
}
