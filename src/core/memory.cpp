
#include "memory.h"
#include <stdlib.h>
#include <cassert>

namespace TEngine { namespace Core {

size_t align(size_t size) {
	size_t aligned = (size + (ALIGN_SIZE - 1)) & ~(ALIGN_SIZE - 1);
	assert(size <= aligned);
	return aligned;
}

void* align(void* p_ptr) {
	word_t ptr = (word_t)p_ptr;
	word_t aligned = (ptr + (ALIGN_SIZE - 1)) & ~(ALIGN_SIZE - 1);
	assert(ptr <= aligned);
	return (void*)aligned;
}

RootAllocator& RootAllocator::getInstance() {
	assert(instance);
	return *instance;
}

RootAllocator::RootAllocator() {
	assert(!instance);
	instance = this;
}

void* RootAllocator::allocate(size_t size) {
	return malloc(align(size));
}

void RootAllocator::free(void* ptr) {
	assert(ptr == align(ptr));
	::free(ptr);
}
}}
