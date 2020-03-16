
#ifndef CORE_H
#define CORE_H

#include "id.h"
#include "component_pool.h"

#include <atomic>
#include <vector>
#include <map>

namespace TEngine::Core {

// Type Unsafe base class of IComponentArray
class TU_IComponentArray {
public:
	virtual bool hasComponent(entity) = 0;
	virtual bool removeComponent(entity) = 0;
	virtual bool removeIfComponent(entity) = 0;
};

template<class T>
class IComponentArray : public TU_IComponentArray {
public:
	virtual ComponentPtr<T> getComponent(entity) = 0;
	virtual ComponentPtr<T> addComponent(entity) = 0;
};

template<class T>
class PooledComponentArray : public IComponentArray<T> {
protected:
	PooledComponentAllocator<T>& m_allocator;
public:
	PooledComponentArray(PooledComponentAllocator<T>& a) : m_allocator(a) { }

	virtual bool hasComponent(entity e) {
		return m_allocator.has(e);
	}

	virtual ComponentPtr<T> getComponent(entity e) {
		return ComponentPtr<T>(m_allocator, e);
	}

	virtual ComponentPtr<T> addComponent(entity e) {
		bool allocated = m_allocator.allocate(e);
		assert(allocated);
		return ComponentPtr<T>(m_allocator, e);
	}

	virtual bool removeComponent(entity e) {
		bool freed = m_allocator.free(e);
		assert(freed);
		return freed;
	}

	virtual bool removeIfComponent(entity e) {
		return m_allocator.has(e) ?
			PooledComponentArray::removeComponent(e) : true;
	}
};

class ComponentManager {
private:
	inline static ComponentManager* instance = nullptr;
	inline static std::atomic_int typeIdCounter = std::atomic_int(1);

	std::vector<TU_IComponentArray*> m_arrays;
public:
	static ComponentManager& getInstance();

	ComponentManager();
	ComponentManager(const ComponentManager&) = delete;
	void operator=(const ComponentManager&) = delete;

	void startUp() { }
	void shutDown() { }
	void update(float) { }

	template<typename T>
	bool registerComponentArray(IComponentArray<T>& arr) {
		int id = getTypeId<T>();
		bool newId = id + 1 == ComponentManager::typeIdCounter;
		assert(newId);

		if (!newId) return false;
		m_arrays.push_back(&arr);
		return true;
	}

	template<typename T>
	bool hasComponent(entity e) {
		return getComponentArray<T>().hasComponent(e);
	}

	template<typename T>
	ComponentPtr<T> getComponent(entity e) {
		return getComponentArray<T>().getComponent(e);
	}

	template<typename T>
	ComponentPtr<T> addComponent(entity e) {
		return getComponentArray<T>().addComponent(e);
	}

	template<typename T>
	bool removeComponent(entity e) {
		return getComponentArray<T>().removeComponent(e);
	}

	template<typename T>
	bool removeIfComponent(entity e) {
		return getComponentArray<T>().removeIfComponent(e);
	}

	void removeAllComponents(entity e) {
		for (auto it = m_arrays.begin(); it != m_arrays.end(); ++it) {
			(*it)->removeIfComponent(e);
		}
	}
private:
	template<typename T>
	int getTypeId() {
		static const int id = typeIdCounter++;
		return id;
	}

	template<typename T>
	IComponentArray<T>& getComponentArray() {
		int id = getTypeId<T>();
		assert(id < typeIdCounter);
		auto ptr = dynamic_cast<IComponentArray<T>*>(m_arrays[id-1]);
		assert(ptr);
		return *ptr;
	}
};

class EntityManager {
private:
	inline static EntityManager* instance = nullptr;
	inline static unsigned int entityIdCounter = 1;

	std::map<unsigned int, bool> m_destroyMap;
public:
	static EntityManager& getInstance();

	EntityManager();
	EntityManager(const EntityManager&) = delete;
	void operator=(const EntityManager&) = delete;

	void startUp() { }
	void shutDown() { }
	void update(float);

	// returns a new entity and increments the counter.
	entity create() {
		return entity(EntityManager::entityIdCounter++);
	}
	// marks entity for destruction next frame
	bool destroy(entity);
};
}
#endif
