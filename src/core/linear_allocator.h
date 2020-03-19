
#ifndef CORE_LINEAR_ALLOCATOR_H
#define CORE_LINEAR_ALLOCATOR_H

#include "memory.h"

namespace TEngine::Core {

class LinearAllocator : public IDerivedAllocator {
private:
	void* m_basePtr = nullptr;
	void* m_topPtr = nullptr;
	size_t m_capacity = 0;
	bool m_running = false;
public:
	LinearAllocator(IRootAllocator& a) : IDerivedAllocator(a) { }

	bool reserve(size_t size);
	void shutDown();

	void* allocate(size_t size);
	void clear();
};
}
#endif
