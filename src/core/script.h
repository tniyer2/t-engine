
#ifndef CORE_SCRIPT_H
#define CORE_SCRIPT_H

#include "id.h"
#include <map>
#include <cassert>

namespace TEngine::Core {

class Script {
public:
	const entity entity;
	Script(Core::entity e) : entity(e) { }

	virtual void Awake() {};
	virtual void Update(float) {};
};

class ScriptManager {
private:
	inline static ScriptManager* instance = nullptr;
	inline static bool running = false;

	std::map<entity, Script*> m_scripts;
	std::map<entity, Script*> m_new;
public:
	static ScriptManager& getInstance();

	ScriptManager();
	ScriptManager(const ScriptManager&) = delete;
	void operator=(const ScriptManager&) = delete;

	void startUp();
	void shutDown();
	void update(float);

	template<class T>
	void addScript(entity e) {
		bool exists = m_scripts.find(e) != m_scripts.end();
		assert(!exists);
		if (exists) return;

		Script* script = new T(e);
		m_scripts[e] = script;
		m_new[e] = script;
	}

	bool hasScript(entity e) {
		return m_scripts.find(e) != m_scripts.end();
	}

	Script* getScript(entity e) {
		auto it = m_scripts.find(e);
		if (it != m_scripts.end()) {
			return it->second;
		}
		else {
			return nullptr;
		}
	}

	void removeScript(entity e) {
		bool removed = pRemoveScript(e);
		assert(removed);
	}

	bool removeIfScript(entity e) {
		return pRemoveScript(e);
	}
private:
	bool pRemoveScript(entity e) {
		auto it = m_scripts.find(e);

		bool exists = it != m_scripts.end();
		if (!exists) return false;

		m_scripts.erase(it);
		auto it2 = m_new.find(e);
		if (it2 != m_new.end()) {
			m_new.erase(it);
		}
		return true;
	}
};
}
#endif
