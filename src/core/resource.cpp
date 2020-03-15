
#include "resource.h"
#include "../utility/boolean.h"
#include <cassert>

namespace TEngine::Core {

ResourceManager& ResourceManager::getInstance() {
	assert(instance);
	return *instance;
}

ResourceManager::ResourceManager()
	: m_allocator(RootAllocator::getInstance()) {
	assert(!instance);
	instance = this;
}

void ResourceManager::startUp() {
	if (!Utility::toggle<true>(running)) return;

	m_allocator.reserve(DEFAULT_STACK_SIZE);
}

void ResourceManager::shutDown() {
	if (!Utility::toggle<false>(running)) return;

	m_allocator.shutDown();
}
}
