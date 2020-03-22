
#include "script_manager.h"

namespace TEngine::Core {

void ScriptManager::shutDown() {
	SubSystem<ScriptManager>::shutDown();

	for (auto it = m_scripts.begin(); it != m_scripts.end(); ++it) {
		delete it->second;
	}
	m_scripts.clear();
	m_newScripts.clear();
}

void ScriptManager::update(float deltaTime) {
	checkRunning();

	for (auto it = m_newScripts.begin(); it != m_newScripts.end(); ++it) {
		it->second->Awake();
	}
	m_newScripts.clear();

	for (auto it = m_scripts.begin(); it != m_scripts.end(); ++it) {
		it->second->Update(deltaTime);
	}
}

bool ScriptManager::hasScript(entity e) {
	checkRunning();
	return m_scripts.find(e) != m_scripts.end();
}

Script* ScriptManager::getScript(entity e) {
	checkRunning();
	auto it = m_scripts.find(e);
	return it != m_scripts.end() ? it->second : nullptr;
}

void ScriptManager::removeScript(entity e) {
	checkRunning();
	bool removed = _removeScript(e);
	assert(removed);
}

bool ScriptManager::removeIfScript(entity e) {
	checkRunning();
	return _removeScript(e);
}

bool ScriptManager::_removeScript(entity e) {
	auto scriptIt = m_scripts.find(e);
	if (scriptIt == m_scripts.end()) return false;
	m_scripts.erase(scriptIt);

	auto newIt = m_newScripts.find(e);
	if (newIt != m_newScripts.end()) m_newScripts.erase(newIt);
	return true;
}
}
