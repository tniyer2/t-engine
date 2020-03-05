
#ifndef CORE_MEMORY_H
#define CORE_MEMORY_H

#include <utility>
#include <iostream>

#include <vector>
#include <unordered_map>

using std::vector;
using std::unordered_map;

namespace TEngine { namespace Core {
	using handle_t = unsigned int;
	using word_t = uintptr_t;
	using bitchar_t = unsigned char;
	static constexpr bitchar_t BITCHAR_MAX = UCHAR_MAX;

	template<class T>
	class data_ptr {
	private:
		unordered_map<handle_t, T*>& m_map;
		unsigned int m_handle;
	public:
		data_ptr(unordered_map<handle_t, T*>& map, handle_t handle) {
			m_map = map;
			m_handle = handle;
		}

		const T* operator->() const { return m_map[m_handle]; }
		T* operator->() { return m_map[m_handle]; }

		const T& operator*() const {
			T* ptr = m_map[m_handle];
			assert(ptr != nullptr);
			return *ptr;
		}
		T& operator*() {
			return const_cast<T&>(*std::as_const(*this));
		}
	};

	size_t align(size_t, size_t);
	void* align(void*, size_t);
	size_t ceil(size_t, size_t);
	bool isPow2(size_t);

	class IAllocator {
	public:
		constexpr static size_t ALIGN_SIZE = sizeof(word_t);
	};

	class IRootAllocator : public IAllocator {
	public:
		virtual void* allocate(size_t) = 0;
		virtual void free(void*) = 0;
	};

	class RootAllocator : public IRootAllocator {
	public:
		void* allocate(size_t) override;
		void free(void*) override;
	};

	class IDerivedAllocator : public IAllocator {
	public:
		IDerivedAllocator(IRootAllocator& a): m_allocator(a) {}
	protected:
		IRootAllocator& m_allocator;
	};

	template<typename T>
	struct PoolHeader {
		PoolHeader<T>* next = nullptr;
		T* arrBlocks = nullptr;
		size_t numBlocks = 0;
		vector<bitchar_t> used;

		PoolHeader(T* blocks, size_t n)
			: arrBlocks(blocks), numBlocks(n), used(ceil(n, 8)) {}
	};

	template<typename T>
	class PoolAllocator : public IDerivedAllocator {
	public:
		constexpr static size_t BLOCK_SIZE = sizeof(T);
		inline const static size_t HEADER_SIZE = align(sizeof(PoolHeader<T>), ALIGN_SIZE);

		PoolAllocator(IRootAllocator& a) : IDerivedAllocator(a) {}
		~PoolAllocator() {
			PoolHeader<T>* cur = m_startPtr;
			while (true) {
				PoolHeader<T>* next = cur->next;
				m_allocator.free(cur);
				cur = next;
				if (cur == nullptr || cur == m_startPtr) break;
			}
		}

		bool reserve(size_t numBlocks) {
			size_t size = HEADER_SIZE + align(numBlocks * BLOCK_SIZE, ALIGN_SIZE);
			void* ptr = m_allocator.allocate(size);
			if (ptr == nullptr) return false;

			T* blockPtr = (T*)(((word_t)ptr) + HEADER_SIZE);
			PoolHeader<T>* headerPtr = new (ptr) PoolHeader<T>(blockPtr, numBlocks);
			if (m_startPtr == nullptr) {
				m_startPtr = headerPtr;
				m_endPtr = headerPtr;
			}
			else {
				m_endPtr->next = headerPtr;
				headerPtr->next = m_startPtr;
				m_endPtr = headerPtr;
			}
			return true;
		}

		T* getBlock() {
			if (m_startPtr == nullptr) return nullptr;

			PoolHeader<T>* cur = m_startPtr;
			while (true) {
				T* block = getBlockInPool(cur);
				if (block != nullptr) {
					return block;
				}
				else if (cur == m_endPtr) {
					return nullptr;
				}
				else {
					cur = cur->next;
				}
			}
		}

		bool freeBlock(T* block) {
			if (m_startPtr == nullptr) return false;
			PoolHeader<T>* cur = m_startPtr;
			while (true) {
				size_t index = block - cur->arrBlocks;
				if (index <= cur->numBlocks) {
					cur->used[index / 8] &= ~(1 << (index % 8));
					return true;
				}
			}
			return false;
		}
	private:
		PoolHeader<T>* m_startPtr = nullptr;
		PoolHeader<T>* m_endPtr = nullptr;

		T* getBlockInPool(PoolHeader<T>* pool) {
			for (unsigned int i = 0; i < pool->used.size(); ++i) {
				char count = useBlockInSet(pool->used[i]);
				if (count != -1) {
					return &(pool->arrBlocks[(i * 8) + count]);
				}
			}
			return nullptr;
		}

		char useBlockInSet(bitchar_t& set) {
			if (set == BITCHAR_MAX) {
				return -1;
			}
			else {
				bitchar_t flag = (set + 1) & ~set; // gets first 0 flag
				assert(isPow2(flag));
				set |= flag;
				char count = 0;
				while ((flag & 1) == 0) {
					flag >>= 1;
					count++;
				}
				assert((flag & 1) == 1);
				assert(count >= 0 && count <= 8);
				return count;
			}
		}
	};
}}
#endif
