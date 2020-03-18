
#ifndef CORE_COMPONENT_MANAGER_H
#define CORE_COMPONENT_MANAGER_H

#include "component_array.h"
#include "entity.h"
#include "subsystem.h"
#include <atomic>
#include <vector>
#include <cassert>

namespace TEngine::Core {

class ComponentManager : public SubSystem<ComponentManager> {
private:
	inline static std::atomic_int typeIdCounter = std::atomic_int(1);
	std::vector<TU_IComponentArray*> m_arrays;
public:
	static std::string typeName() { return "Core::ComponentManager"; }

	using SubSystem<ComponentManager>::SubSystem;

	template<class T>
	void registerComponentArray(IComponentArray<T>& arr) {
		checkRunning();

		int id = getTypeId<T>();
		bool newId = id == ComponentManager::typeIdCounter - 1;
		assert(newId);
		if (!newId) return;

		m_arrays.push_back(&arr);
	}

	template<class T>
	bool hasComponent(entity e) {
		checkRunning();
		return getComponentArray<T>().hasComponent(e);
	}

	template<class T>
	ComponentPtr<T> getComponent(entity e) {
		checkRunning();
		return getComponentArray<T>().getComponent(e);
	}

	template<class T>
	ComponentPtr<T> addComponent(entity e) {
		checkRunning();
		return getComponentArray<T>().addComponent(e);
	}

	template<class T>
	bool removeComponent(entity e) {
		checkRunning();
		return getComponentArray<T>().removeComponent(e);
	}

	template<class T>
	bool removeIfComponent(entity e) {
		checkRunning();
		return getComponentArray<T>().removeIfComponent(e);
	}

	void removeComponents(entity e) {
		checkRunning();
		for (auto it = m_arrays.begin(); it != m_arrays.end(); ++it) {
			(*it)->removeIfComponent(e);
		}
	}
private:
	template<typename T>
	static int getTypeId() {
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
}
#endif
