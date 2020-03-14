
#include "stack.h"
#include "../utility/boolean.h"

#include <stdlib.h>
#include <cstring>
#include <cassert>

namespace TEngine::Core {

bool LinearAllocator::reserve(size_t size) {
	if (!Utility::toggle<true>(m_running)) return false;

	size = align(size);
	void* ptr = malloc(size);
	if (!ptr) return false;

	m_basePtr = ptr;
	m_topPtr = ptr;
	m_capacity = size;
	return true;
}

void LinearAllocator::shutDown() {
	if (!Utility::toggle<false>(m_running)) return;

	m_allocator.free(m_basePtr);
}

void* LinearAllocator::allocate(size_t size) {
	assert(m_running);
	if (!m_running) return nullptr;

	size = align(size);
	size_t available = m_capacity - ((size_t)m_topPtr - (size_t)m_basePtr);
	if (size > available) return nullptr;

	void* cur = m_topPtr;
	m_topPtr = (void*)((size_t)m_topPtr + size);
	return cur;
}

void LinearAllocator::clear() {
	assert(m_running);
	if (!m_running) return;

	std::memset(m_basePtr, 0, m_capacity);
	m_topPtr = m_basePtr;
}
}
