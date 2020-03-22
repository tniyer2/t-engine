
#include "core.h"

namespace TEngine::Core {

template<class T>
entity create() {
	return EntityManager::getInstance().create();
}

template<>
void destroy<entity>(entity id) {
	EntityManager::getInstance().destroy(id);
}

void removeScript(entity id) {
	ScriptManager::getInstance().removeScript(id);
}
}
