
#include <cassert>
#include "memory.h"

namespace TEngine { namespace Core {

size_t align(size_t size, size_t alignSize) {
	assert(isPow2(alignSize));
	alignSize -= 1;
	size_t aligned = (size + alignSize) & ~alignSize;
	assert(size <= aligned);
	return aligned;
}

void* align(void* p_ptr, size_t p_alignSize) {
	assert(isPow2(p_alignSize));
	word_t ptr = (word_t)p_ptr;
	word_t alignSize = p_alignSize - 1;
	word_t aligned = (ptr + alignSize) & ~alignSize;
	assert(ptr <= aligned);
	return (void*)aligned;
}

size_t ceil(size_t n, size_t d) {
	assert(d != 0);
	return (n + d - 1) / d;
}

bool isPow2(size_t n) {
	return n != 0 && (n & (n - 1)) == 0;
}

void* RootAllocator::allocate(size_t size) {
	size = align(size, ALIGN_SIZE);
	return malloc(size);
}

void RootAllocator::free(void* ptr) {
	::free(ptr);
}
}}
