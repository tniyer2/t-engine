
#ifndef CORE_H
#define CORE_H

#include <atomic>
#include <unordered_map>
using std::unordered_map;
#include <vector>
using std::vector;

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

class IComponent {
public:
	IComponent(Entity e) : m_entity(e) {};
private:
	Entity m_entity;
};

class IComponentArray {};

template<typename T>
class ComponentArray : public IComponentArray {
public:
	ComponentArray(vector<T>& data) : m_array(data) {}

	T& getComponent(Entity e) {
		return m_array[e.id];
	}

	T& createComponent(Entity e, T comp) {
		m_array.push_back(comp);
		return comp;
	}
private:
	vector<T>& m_array;
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
	void registerView(IComponentArray* view) {
		m_map[getTypeId<T>()] = view;
	}

	template<typename T>
	T& getComponent(Entity e) {
		return getComponentArray<T>()->getComponent(e);
	}

	template<typename T>
	T& createComponent(Entity e, T comp) {
		return getComponentArray<T>()->createComponent(e, comp);
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
