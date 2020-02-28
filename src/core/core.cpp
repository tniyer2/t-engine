
#include "core.h"

namespace TEngine { namespace Core {

template<class T, class U>
void ComponentManager::registerView(U view) {
	m_map[getTypeId(T)] = view;
}

template<class T>
T& ComponentManager::getComponent(Entity e) {
	return (T)m_map.find(getTypeId(T))->getComponent(e);
}

template<class T>
T& ComponentManager::createComponent(Entity e) {
	return (T)m_map.find(getTypeId(T))->createComponent(e);
}
}}
