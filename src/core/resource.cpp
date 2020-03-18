
#include "resource.h"
#include "../utility/boolean.h"
#include <cassert>

namespace TEngine::Core {

ResourceManager& ResourceManager::getInstance() {
	assert(s_instance);
	return *s_instance;
}

ResourceManager::ResourceManager()
	: m_allocator(RootAllocator::getInstance()) {
	assert(!s_instance);
	s_instance = this;
}

void ResourceManager::startUp() {
	if (!Utility::toggle<true>(s_running)) return;

	m_allocator.reserve(DEFAULT_STACK_SIZE);
}

void ResourceManager::shutDown() {
	if (!Utility::toggle<false>(s_running)) return;

	m_allocator.shutDown();
}
}
