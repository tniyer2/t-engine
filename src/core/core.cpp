
#include "core.h"
#include "script.h"

namespace TEngine::Core {

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
