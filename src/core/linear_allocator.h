
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
	using IDerivedAllocator::IDerivedAllocator;

	bool isRunning() const { return m_running; }
	size_t getUsed() const { return ((size_t)m_topPtr - (size_t)m_basePtr); }
	size_t getAvailable() const { return m_capacity - getUsed(); }

	void reserve(size_t size);
	void freeAll(); // frees all allocated memory

	void* allocate(size_t size);
	void clear(); // resets pointer to base
};
}
#endif
