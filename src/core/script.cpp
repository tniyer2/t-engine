
#include "script.h"
#include "../utility/boolean.h"
#include <cassert>

namespace TEngine::Core {

ScriptManager& ScriptManager::getInstance() {
	assert(instance);
	return *instance;
}

ScriptManager::ScriptManager() {
	assert(!instance);
	instance = this;
}

void ScriptManager::startUp() {
	if (!Utility::toggle<true>(running)) return;
}

void ScriptManager::shutDown() {
	if (!Utility::toggle<false>(running)) return;

	for (auto it = m_scripts.begin(); it != m_scripts.end(); ++it) {
		delete it->second;
	}
	m_scripts.clear();
	m_new.clear();
}

void ScriptManager::update(float deltaTime) {
	assert(running);
	if (!running) return;

	for (auto it = m_new.begin(); it != m_new.end(); ++it) {
		it->second->Awake();
	}
	m_new.clear();

	for (auto it = m_scripts.begin(); it != m_scripts.end(); ++it) {
		it->second->Update(deltaTime);
	}
}
}
