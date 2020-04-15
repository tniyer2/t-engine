
#include "linear_allocator.h"

#include <cstring>
#include <cassert>

namespace TEngine::Core {

void LinearAllocator::reserve(size_t size) {
	if (m_reserved) {
		throw "Invalid Call. Memory already reserved.";
	}

	size = align(size);
	void* ptr = m_allocator.allocate(size);
	if (!ptr) {
		throw "Could not allocate memory.";
	}

	m_basePtr = ptr;
	m_topPtr = ptr;
	m_capacity = size;
	m_reserved = true;
}

void LinearAllocator::freeAll() {
	if (!m_reserved) {
		throw "Invalid Call. No memory reserved to free.";
	}

	m_allocator.free(m_basePtr);
	m_reserved = false;
}

void LinearAllocator::checkRunning() const {
	if (!m_reserved) {
		throw "Invalid Call. Memory not reserved.";
	}
}

void* LinearAllocator::allocate(size_t size) {
	checkRunning();

	size = align(size);
	if (size > getAvailable()) return nullptr;

	void* cur = m_topPtr;
	m_topPtr = (void*)((size_t)m_topPtr + size);
	return cur;
}

void LinearAllocator::clear() {
	checkRunning();

	std::memset(m_basePtr, 0, m_capacity);
	m_topPtr = m_basePtr;
}
}
