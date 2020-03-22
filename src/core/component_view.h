
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
	using Arrays = std::array<TU_IComponentArray*, sizeof...(Ts)>;
	using TypedArrays = std::tuple<IComponentArray<Ts>*...>;
private:
	inline static unsigned int typeIdCounter = 0;

	Arrays m_arrays;
	TypedArrays m_typedArrays;
	TU_IComponentIterator* m_it = nullptr;
	const unsigned int m_shortest_index = 0;

	template<class T>
	static unsigned int getTypeId() {
		const static unsigned int id = typeIdCounter++;
		return id;
	}

	static unsigned int findShortest(Arrays& arrays) {
		size_t shortest_count = std::numeric_limits<size_t>::max();
		unsigned int shortest_index = 0;
		for (unsigned int i = 0; i < arrays.size(); ++i) {
			size_t count = arrays[i]->getCount();
			if (count < shortest_count) {
				shortest_count = count;
				shortest_index = i;
			}
		}
		return shortest_index;
	}

	template<class T>
	ComponentPtr<T> getComponentPtr(entity id, bool& allPresent) {
		if (allPresent) {
			auto c = std::get<IComponentArray<T>*>(m_typedArrays)->getComponent(id);
			allPresent = (bool)c;
			return allPresent ? c : ComponentPtr<T>();
		}
		else {
			return ComponentPtr<T>();
		}
	}

	ComponentView(ComponentManager& m)
		: m_arrays({ &m.getComponentArray<Ts>()... }),
		m_typedArrays({ &m.getComponentArray<Ts>()... }),
		m_shortest_index(findShortest(m_arrays)) {

		(getTypeId<Ts>(), ...);
		m_it = m_arrays[m_shortest_index]->TUbegin().release();
	}
public:
	ComponentView() : ComponentView(ComponentManager::getInstance()) { }
	~ComponentView() { delete m_it; }

	explicit operator bool() const { return m_it->operator bool(); }

	std::optional<std::tuple<ComponentPtr<Ts>...>> next() {
		entity id = m_it->getEntity();
		bool allPresent = true;
		std::tuple<ComponentPtr<Ts>...> components =
			{ getComponentPtr<Ts>(id, allPresent)... };

		m_it->operator++();
		if (allPresent) return components; else return {};
	}
};
}
#endif
