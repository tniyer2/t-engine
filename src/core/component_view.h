
#ifndef CORE_COMPONENT_VIEW_H
#define CORE_COMPONENT_VIEW_H

#include "component_manager.h"
#include "component_array.h"
#include "component_iterator.h"
#include <array>
#include <tuple>
#include <optional>
#include <limits>

namespace TEngine::Core {

template<class ...Ts>
struct ComponentView {
	using Arrays = std::array<UntypedComponentArray*, sizeof...(Ts)>;
	using TypedArrays = std::tuple<IComponentArray<Ts>*...>;
private:
	Arrays m_arrays;
	TypedArrays m_typedArrays;
	UntypedComponentIterator* m_it = nullptr;
	const unsigned int m_shortestIndex = 0;

	ComponentView(ComponentManager& m)
		: m_arrays({ &m.getComponentArray<Ts>()... }),
		m_typedArrays({ &m.getComponentArray<Ts>()... }),
		m_shortestIndex(findShortest(m_arrays)) {

		m_it = m_arrays[m_shortestIndex]->untypedBegin().release();
	}
public:
	ComponentView() : ComponentView(ComponentManager::getInstance()) { }
	~ComponentView() { delete m_it; }

	explicit operator bool() const { return (bool)*m_it; }

	std::optional<std::tuple<ComponentHandle<Ts>...>> next() {
		entity id = m_it->getEntity();
		bool allPresent = true;

		std::tuple<ComponentHandle<Ts>...> components =
			{ getHandleIfAllPresent<Ts>(id, allPresent)... };

		m_it->operator++();
		if (allPresent) return components;
		else return {};
	}
private:
	static unsigned int findShortest(Arrays& arrays) {
		size_t shortestCount = std::numeric_limits<size_t>::max();
		unsigned int shortestIndex = 0;

		for (unsigned int i = 0; i < arrays.size(); ++i) {
			size_t count = arrays[i]->getCount();
			if (count < shortestCount) {
				shortestCount = count;
				shortestIndex = i;
			}
		}

		return shortestIndex;
	}

	template<class T>
	ComponentHandle<T> getHandleIfAllPresent(entity id, bool& allPresent) {
		if (allPresent) {
			auto handle = std::get<IComponentArray<T>*>(m_typedArrays)->getComponent(id);
			allPresent = (bool)handle;
			return handle;
		}
		else {
			return ComponentHandle<T>::invalid();
		}
	}
};
}
#endif
