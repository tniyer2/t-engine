
#ifndef CORE_RESOURCE_HANDLE_H
#define CORE_RESOURCE_HANDLE_H

namespace TEngine::Core {

template<class T>
class ResourceHandle {
private:
	T* m_ptr = nullptr;
public:
	static ResourceHandle invalid() { return ResourceHandle(); }

	ResourceHandle() { }
	ResourceHandle(T* ptr) : m_ptr(ptr) { }

	explicit operator bool() const { return (bool)m_ptr; }

	const T* operator->() const { return m_ptr; }
	T* operator->() { return m_ptr; }

	const T& operator*() const {
		if (!m_ptr) {
			throw "ResourceHandle cannot dereference resource.";
		}
		return *m_ptr;
	}
	T& operator*() {
		return const_cast<T&>(static_cast<const ResourceHandle<T>&>(*this).operator*());
	}
};
}
#endif
