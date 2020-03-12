
#ifndef CORE_MEMORY_H
#define CORE_MEMORY_H

#include <cassert>

namespace TEngine::Core {
	using word_t = uintptr_t;

	static constexpr size_t ALIGN_SIZE = sizeof(word_t);
	size_t align(size_t);
	void* align(void*);

	class IAllocator {};

	class IRootAllocator : public IAllocator {
	public:
		virtual void startUp() { };
		virtual void shutDown() { };
		void update(float) { }

		virtual void* allocate(size_t) = 0;
		virtual void free(void*) = 0;
	};

	class RootAllocator : public IRootAllocator {
	private:
		inline static RootAllocator* instance = nullptr;
	public:
		inline static RootAllocator& getInstance() {
			assert(instance);
			return *instance;
		}

		RootAllocator() {
			assert(!instance);
			instance = this;
		}
		RootAllocator(const RootAllocator&) = delete;
		void operator=(const RootAllocator&) = delete;

		void* allocate(size_t) override;
		void free(void*) override;
	};

	class IDerivedAllocator : public IAllocator {
	public:
		IDerivedAllocator(IRootAllocator& a) : m_allocator(a) {}
	protected:
		IRootAllocator& m_allocator;
	};
}
#endif
