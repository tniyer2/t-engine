
#ifndef CORE_RESOURCE_PTR_H
#define CORE_RESOURCE_PTR_H

#include <cassert>

namespace TEngine::Core {

template<class T>
class ResourcePtr {
private:
	T* m_ptr = nullptr;
public:
	ResourcePtr() { }
	ResourcePtr(T* ptr) : m_ptr(ptr) { }

	operator bool() const { return m_ptr; }

	T* operator->() {
		assert(m_ptr); return m_ptr;
	}
	const T* operator->() const {
		assert(m_ptr); return m_ptr;
	}

	T& operator*() {
		assert(m_ptr); return *m_ptr;
	}
	const T& operator*() const {
		assert(m_ptr); return *m_ptr;
	}
};
}
#endif
