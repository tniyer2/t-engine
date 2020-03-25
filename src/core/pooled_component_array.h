
#ifndef CORE_POOLED_COMPONENT_ARRAY_H
#define CORE_POOLED_COMPONENT_ARRAY_H

#include "pooled_component_allocator.h"
#include "component_array.h"
#include "component_handle.h"
#include "entity.h"

namespace TEngine::Core {

template<class T>
class PooledComponentArray : public IComponentArray<T> {
	using Iterator = typename PooledComponentAllocator<T>::PooledComponentIterator;
protected:
	PooledComponentAllocator<T>& m_allocator;
public:
	PooledComponentArray(PooledComponentAllocator<T>& a) : m_allocator(a) { }

	size_t getCount() const override {
		return m_allocator.getCount();
	}

	bool hasComponent(entity id) const override {
		return m_allocator.has(id);
	}

	ComponentHandle<T> getComponent(entity id) const override {
		return ComponentHandle<T>(m_allocator, id);
	}

	std::unique_ptr<UntypedComponentIterator> untypedBegin() override {
		return std::unique_ptr<UntypedComponentIterator>(_begin());
	}

	std::unique_ptr<IComponentIterator<T>> begin() override {
		return std::unique_ptr<IComponentIterator<T>>(_begin());
	}

	ComponentHandle<T> addComponent(entity id) override {
		if (m_allocator.allocate(id)) {
			return ComponentHandle<T>(m_allocator, id);
		}
		else {
			return ComponentHandle<T>::invalid();
		}
	}

	void removeComponent(entity id) override {
		_removeComponent(id);
	}

	bool removeIfComponent(entity id) override {
		if (m_allocator.has(id)) {
			_removeComponent(id);
			return true;
		}
		else {
			return false;
		}
	}
protected:
	virtual void _removeComponent(entity id) {
		if (!m_allocator.free(id)) {
			throw "Could not remove component.";
		}
	}
private:
	IComponentIterator<T>* _begin() const {
		return new Iterator(m_allocator.begin());
	}
};
}
#endif
