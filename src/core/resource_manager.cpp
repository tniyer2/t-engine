
#include "resource_manager.h"

namespace TEngine::Core {

void ResourceManager::startUp() {
	SubSystem<ResourceManager>::toggleStartUp();

	m_allocator.reserve(DEFAULT_STACK_SIZE);
}

void ResourceManager::shutDown() {
	SubSystem<ResourceManager>::toggleShutDown();

	m_allocator.freeAll();
}
}
