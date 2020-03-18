
#ifndef CORE_SCRIPT_MANAGER_H
#define CORE_SCRIPT_MANAGER_H

#include "script.h"
#include "entity.h"
#include "subsystem.h"
#include <map>
#include <cassert>

namespace TEngine::Core {

class ScriptManager : public SubSystem<ScriptManager> {
private:
	std::map<entity, Script*> m_scripts; // holds all scripts
	std::map<entity, Script*> m_newScripts; // holds scripts not called Awake on
public:
	static std::string typeName() { return "Core::ScriptManager"; }

	using SubSystem<ScriptManager>::SubSystem;

	void shutDown();
	void update(float); // calls awake on new scripts and update on all scripts.

	// adds script to entity. asserts entity has no script attached.
	template<class T>
	void addScript(entity e) {
		checkRunning();

		bool exists = hasScript(e);
		assert(!exists);
		if (exists) return;

		Script* script = new T(e);
		m_scripts[e] = script;
		m_newScripts[e] = script;
	}

	bool hasScript(entity);
	Script* getScript(entity);
	void removeScript(entity); // asserts removal.
	bool removeIfScript(entity); // does not assert removal.
private:
	bool _removeScript(entity); // removes script and returns if removed.
};
}
#endif
