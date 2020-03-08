
#ifndef CORE_H
#define CORE_H

#include <atomic>

#include <unordered_map>
#include <map>
#include <vector>
using std::unordered_map;
using std::map;
using std::vector;

#include "id.h"
#include "memory.h"

namespace TEngine { namespace Core {

class EntityManager {
public:
	EntityManager() {}
	EntityManager(const EntityManager&) = delete;
	void operator=(const EntityManager&) = delete;

	void startUp() {}
	void shutDown() {}

	entity createEntity() {
		return entity(++m_entityIdCounter);
	}
private:
	unsigned int m_entityIdCounter = 0;
};

class IComponent {
public:
	entity entity;
};

class IComponentArray {};

template<typename T>
class ComponentArray : public IComponentArray {
public:
	ComponentArray() {}

	T* getComponent(entity e) {
		return nullptr;
	}

	T* addComponent(entity e) {
		return nullptr;
	}
};

class ComponentManager {
public:
	ComponentManager() {}
	~ComponentManager() {}

	void startUp() {}
	void shutDown() {}

	ComponentManager(const ComponentManager&) = delete;
	void operator=(const ComponentManager&) = delete;

	template<typename T>
	void registerComponentArray(IComponentArray* arr) {
		m_map[getTypeId<T>()] = arr;
	}

	template<typename T>
	T* getComponent(entity e) {
		return getComponentArray<T>()->getComponent(e);
	}

	template<typename T>
	T* setComponent(entity e) {
		return getComponentArray<T>()->setComponent(e);
	}
private:
	static std::atomic_int typeIdCounter;
	unordered_map<int, IComponentArray*> m_map;

	template<typename T>
	int getTypeId() {
		static const int id = ++typeIdCounter;
		return id;
	}

	template<typename T>
	ComponentArray<T>* getComponentArray() {
		return static_cast<ComponentArray<T>*>(m_map[getTypeId<T>()]);
	}
};
}}
#endif
 