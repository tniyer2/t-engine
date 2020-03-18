
#ifndef CORE_RESOURCE_H
#define CORE_RESOURCE_H

#include "stack.h"
#include <string>

namespace TEngine::Core {

class IResource {
public:
	using Loader = void;
};

template<class T>
class ResourcePtr {
private:
	T* m_ptr = nullptr;
public:
	ResourcePtr() { }
	ResourcePtr(T* ptr) : m_ptr(ptr) { }

	operator bool() { return m_ptr; }

	T* operator->() {
		assert(m_ptr);
		return m_ptr;
	}
	const T* operator->() const {
		assert(m_ptr);
		return m_ptr;
	}

	T& operator*() {
		assert(m_ptr);
		return *m_ptr;
	}
	const T& operator*() const {
		assert(m_ptr);
		return *m_ptr;
	}
};

class ResourceManager {
private:
	constexpr static size_t DEFAULT_STACK_SIZE = 10485760; // 10 MB
	inline static ResourceManager* s_instance = nullptr;
	inline static bool s_running = false;

	LinearAllocator m_allocator;
public:
	static ResourceManager& getInstance();

	ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	void operator=(const ResourceManager&) = delete;

	void startUp();
	void shutDown();

	template<class T>
	ResourcePtr<T> load(std::string filePath) {
		return ResourcePtr<T>();
	}
};
}
#endif
