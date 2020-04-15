
#ifndef CORE_SCRIPT_MANAGER_H
#define CORE_SCRIPT_MANAGER_H

#include "script.h"
#include "entity.h"
#include "subsystem.h"
#include <map>
#include <string>
#include <cassert>

namespace TEngine::Core {

class ScriptManager : public SubSystem<ScriptManager> {
private:
	std::map<entity, Script*> m_allScripts;
	std::map<entity, Script*> m_newScripts;
public:
	static std::string typeName() { return "Core::ScriptManager"; }

	using SubSystem<ScriptManager>::SubSystem;

	void startUp() { SubSystem<ScriptManager>::toggleStartUp(); }
	void shutDown();
	void update(float);

	bool hasScript(entity) const;
	Script* getScript(entity) const;

	template<class T>
	void addScript(entity id) {
		checkRunning();

		if (hasScript(id)) {
			throw "Invalid Argument. Script for id already exists.";
		}

		Script* script = new T(id);
		m_allScripts[id] = script;
		m_newScripts[id] = script;
	}

	void removeScript(entity);
	bool removeIfScript(entity);
private:
	bool _removeScript(entity);
};
}
#endif
