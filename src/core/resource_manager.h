
#ifndef CORE_RESOURCE_MANAGER_H
#define CORE_RESOURCE_MANAGER_H

#include "resource_ptr.h"
#include "linear_allocator.h"
#include "subsystem.h"
#include <string>

namespace TEngine::Core {

class ResourceManager : public SubSystem<ResourceManager> {
private:
	constexpr static size_t DEFAULT_STACK_SIZE = 10485760; // 10 MB

	LinearAllocator m_allocator;
public:
	static std::string typeName() { return "Core::ResourceManager"; }

	ResourceManager() :
		SubSystem<ResourceManager>(),
		m_allocator(RootAllocator::getInstance()) { };

	void startUp();
	void shutDown();

	template<class T>
	ResourcePtr<T> load(std::string filePath) {
		return ResourcePtr<T>();
	}
};
}
#endif
