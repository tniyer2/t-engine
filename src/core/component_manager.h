
#ifndef CORE_COMPONENT_MANAGER_H
#define CORE_COMPONENT_MANAGER_H

#include "component_array.h"
#include "component_handle.h"
#include "entity.h"
#include "subsystem.h"
#include <atomic>
#include <vector>
#include <string>
#include <cassert>

namespace TEngine::Core {

class ComponentManager : public SubSystem<ComponentManager> {
private:
	inline static std::atomic_int typeIdCounter = std::atomic_int(1);
	std::vector<UntypedComponentArray*> m_arrays;
public:
	static std::string typeName() { return "Core::ComponentManager"; }

	using SubSystem<ComponentManager>::SubSystem;

	template<class T>
	IComponentArray<T>& getComponentArray() const {
		checkRunning();

		int counter = typeIdCounter;
		int id = getTypeId<T>();
		if (counter != typeIdCounter) {
			throw ("Invalid Call. ComponentArray is not registered for type " + getTypeName<T>());
		}

		auto ptr = dynamic_cast<IComponentArray<T>*>(m_arrays[(size_t)id - 1]);
		assert(ptr);
		return *ptr;
	}

	template<class T>
	bool hasComponent(entity id) const {
		checkRunning();
		return getComponentArray<T>().hasComponent(id);
	}

	template<class T>
	ComponentHandle<T> getComponent(entity id) const {
		checkRunning();
		return getComponentArray<T>().getComponent(id);
	}

	template<class T>
	auto begin() const {
		checkRunning();
		return getComponentArray<T>().begin();
	}

	template<class T>
	void registerComponentArray(IComponentArray<T>& arr) {
		checkRunning();

		int id = getTypeId<T>();
		if (id != ComponentManager::typeIdCounter - 1) {
			throw ("Invalid Call. ComponentArray is already registered for type " + getTypeName<T>());
		}

		m_arrays.push_back(&arr);
	}

	template<class T>
	ComponentHandle<T> addComponent(entity id) {
		checkRunning();
		return getComponentArray<T>().addComponent(id);
	}

	template<class T>
	void removeComponent(entity id) {
		checkRunning();
		getComponentArray<T>().removeComponent(id);
	}

	template<class T>
	bool removeIfComponent(entity id) {
		checkRunning();
		return getComponentArray<T>().removeIfComponent(id);
	}

	void removeComponents(entity id) {
		checkRunning();
		for (auto it = m_arrays.begin(); it != m_arrays.end(); ++it) {
			(*it)->removeIfComponent(id);
		}
	}
private:
	template<class T>
	static int getTypeId() {
		static const int id = typeIdCounter++;
		return id;
	}

	template<class T>
	constexpr static std::string getTypeName() {
		return std::string(typeid(T).name());
	}
};
}
#endif
