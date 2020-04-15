
#include "script_manager.h"

namespace TEngine::Core {

void ScriptManager::shutDown() {
	SubSystem<ScriptManager>::toggleShutDown();

	for (auto it = m_allScripts.begin(); it != m_allScripts.end(); ++it) {
		delete it->second;
	}
	m_allScripts.clear();
	m_newScripts.clear();
}

void ScriptManager::update(float deltaTime) {
	checkRunning();

	for (auto it = m_newScripts.begin(); it != m_newScripts.end(); ++it) {
		it->second->Awake();
	}

	for (auto it = m_allScripts.begin(); it != m_allScripts.end(); ++it) {
		if (m_newScripts.find(it->first) != m_newScripts.end()) continue;
		it->second->Update(deltaTime);
	}

	m_newScripts.clear();
}

bool ScriptManager::hasScript(entity id) const {
	checkRunning();
	return m_allScripts.find(id) != m_allScripts.end();
}

Script* ScriptManager::getScript(entity id) const {
	checkRunning();
	auto it = m_allScripts.find(id);
	if (it != m_allScripts.end()) return it->second;
	else return nullptr;
}

void ScriptManager::removeScript(entity id) {
	checkRunning();
	_removeScript(id);
}

bool ScriptManager::removeIfScript(entity id) {
	checkRunning();
	if (hasScript(id)) {
		_removeScript(id);
		return true;
	}
	else {
		return false;
	}
}

bool ScriptManager::_removeScript(entity id) {
	auto allIt = m_allScripts.find(id);
	if (allIt == m_allScripts.end()) return false;
	m_allScripts.erase(allIt);

	auto newIt = m_newScripts.find(id);
	if (newIt != m_newScripts.end()) m_newScripts.erase(newIt);
	return true;
}
}
