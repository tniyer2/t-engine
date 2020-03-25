
#include "linear_allocator.h"

#include "../utility/boolean.h"
#include <cstring>
#include <cassert>

namespace TEngine::Core {

void LinearAllocator::reserve(size_t size) {
	if (!Utility::toggle<true>(m_running)) {
		throw "Invalid Call. LinearAllocator is already running.";
	}

	size = align(size);
	void* ptr = m_allocator.allocate(size);
	if (!ptr) {
		throw "Could not allocate memory.";
	}

	m_basePtr = ptr;
	m_topPtr = ptr;
	m_capacity = size;
}

void LinearAllocator::freeAll() {
	if (!Utility::toggle<false>(m_running)) {
		throw "Invalid Call. LinearAllocator is not running.";
	}
	m_allocator.free(m_basePtr);
}

void* LinearAllocator::allocate(size_t size) {
	if (!m_running) {
		throw "Invalid Call. LinearAllocator is not running.";
	}

	size = align(size);
	if (size > getAvailable()) return nullptr;

	void* cur = m_topPtr;
	m_topPtr = (void*)((size_t)m_topPtr + size);
	return cur;
}

void LinearAllocator::clear() {
	if (!m_running) {
		throw "Invalid Call. LinearAllocator is not running.";
	}

	std::memset(m_basePtr, 0, m_capacity);
	m_topPtr = m_basePtr;
}
}
