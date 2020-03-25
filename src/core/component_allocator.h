
#ifndef CORE_COMPONENT_ALLOCATOR_H
#define CORE_COMPONENT_ALLOCATOR_H

#include "memory.h"
#include "entity.h"

namespace TEngine::Core {

template<class T>
class IComponentAllocator : public IDerivedAllocator {
protected:
	size_t m_count = 0;
public:
	using IDerivedAllocator::IDerivedAllocator;

	size_t getCount() const { return m_count; }

	virtual bool has(entity) const = 0;
	virtual T* get(entity) const = 0;

	virtual bool allocate(entity) = 0;
	virtual bool free(entity) = 0;
};
}
#endif
