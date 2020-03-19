
#include "resource_manager.h"

namespace TEngine::Core {

void ResourceManager::startUp() {
	SubSystem<ResourceManager>::startUp();
	m_allocator.reserve(DEFAULT_STACK_SIZE);
}

void ResourceManager::shutDown() {
	SubSystem<ResourceManager>::shutDown();
	m_allocator.shutDown();
}
}
