
#ifndef CORE_CORE_H
#define CORE_CORE_H

#include "component_manager.h"
#include "entity_manager.h"
#include "script_manager.h"

namespace TEngine::Core {

template<class T>
ComponentPtr<T> create(entity id) {
	return ComponentManager::getInstance().addComponent<T>(id);
}

template<class T>
void destroy(entity id) {
	ComponentManager::getInstance().removeComponent<T>(id);
}

template<class T>
entity create();

template entity create<entity>();

template<>
void destroy<entity>(entity);

template<class T>
void addScript(entity id) {
	ScriptManager::getInstance().addScript<T>(id);
}

void removeScript(entity);
}
#endif
