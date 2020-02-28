
#ifndef CORE_H
#define CORE_H

#include <atomic>
#include <unordered_map>
using std::unordered_map;

namespace TEngine { namespace Core {

struct Entity {
	unsigned int id;
};

class EntityManager {
public:
	EntityManager() {}
	~EntityManager() {}

	void startUp() {}
	void shutDown() {}

	EntityManager(const EntityManager&) = delete;
	void operator=(const EntityManager&) = delete;

	Entity createEntity() {
		return { m_entityIdCounter++ };
	}
private:
	unsigned int m_entityIdCounter = 0;
};

class AbstractComponent {
public:
	AbstractComponent(Entity e) : m_entity(e) {};
private:
	Entity m_entity;
};

class ComponentManager {
public:
	ComponentManager() {}
	~ComponentManager() {}

	void startUp() {}
	void shutDown() {}

	ComponentManager(const ComponentManager&) = delete;
	void operator=(const ComponentManager&) = delete;

	template<class T, class U>
	void registerView(U);

	template<class T>
	T& getComponent(Entity);

	template<class T>
	T& createComponent(Entity);
private:
	static std::atomic_int typeIdCounter;
	unordered_map<int, void*> m_map;

	template <typename T>
	int getTypeId() {
		static const int id = ++typeIdCounter;
		return id;
	}
};
}}
#endif
