
#ifndef POOL_H
#define POOL_H

#include "id.h"
#include "memory.h"

namespace TEngine { namespace Core {
	using index_t = unsigned short;
	static constexpr index_t INVALID_INDEX = (index_t)-1;

	template<typename T>
	class PoolAllocator;

	template<typename T>
	class Component {
	private:
		PoolAllocator<T>& m_allocator;
		entity m_entity;
	public:
		Component(PoolAllocator<T>& a, entity e)
			: m_allocator(a), m_entity(e) {
			assert(m_entity != 0);
		}

		T* operator->() {
			return m_allocator.get(m_entity);
		}
		const T* operator->() const {
			return m_allocator.get(m_entity);
		}

		T& operator*() {
			T* ptr = m_allocator.get(m_entity);
			assert(ptr);
			return *ptr;
		}
		const T& operator*() const {
			T* ptr = m_allocator.get(m_entity);
			assert(ptr);
			return *ptr;
		}
	};

	template<typename T>
	class PoolAllocator : public IDerivedAllocator {
		struct Bucket {
			entity key;
			index_t value = 0;
			index_t prev = 0;
			index_t next = 0;
			bool used = false;

			void set(entity p_key, index_t p_value, index_t p_prev, index_t p_next) {
				key = p_key;
				value = p_value;
				prev = p_prev;
				next = p_next;
			}

			void set(const Bucket& b) {
				key = b.key;
				value = b.value;
				prev = b.prev;
				next = b.next;
			}
		};

		struct FreeBlock {
			index_t next = INVALID_INDEX;
			index_t prev = INVALID_INDEX;
		};

		template<typename T>
		struct Header {
			Header<T>* next = nullptr;
			Header<T>* prev = nullptr;
			Bucket* pBuckets = nullptr;
			T* pBlocks = nullptr;
			index_t freeBucket = 0;
			index_t freeBlock = INVALID_INDEX;
			index_t top = 0;
			index_t numBlocks = 0;

			Header(Bucket* p1, T* p2, index_t n)
				: pBuckets(p1), pBlocks(p2), numBlocks(n) {}
		};

		template<typename T>
		class Iterator {
		private:
			PoolAllocator<T>& m_allocator;
			Header<T>* m_pool = nullptr;
		public:
			Iterator(PoolAllocator<T>& a, Header<T>* pool)
				: m_allocator(a), m_pool(pool) {}

			Iterator<T>& operator++() {
				assert(m_pool);
				if (m_pool == m_allocator.m_pEnd) {
					m_pool = nullptr;
				}
				else {
					auto ptr = m_pool->next;
					assert(ptr);
					m_pool = ptr;
				}
				return *this;
			}
			Iterator<T> operator++(int) {
				Iterator<T> temp(*this);
				operator++();
				return temp;
			}

			Iterator<T>& operator--() {
				assert(m_pool != m_allocator.m_pStart);
				if (m_pool == nullptr) {
					m_pool = m_allocator.m_pEnd;
				}
				else {
					auto ptr = m_pool->prev;
					assert(ptr);
					m_pool = ptr;
				}
				return *this;
			}
			Iterator<T> operator--(int) {
				Iterator<T> temp(*this);
				operator--();
				return temp;
			}

			bool isEnd() { return m_pool == nullptr; }
			bool isEnd() const { return m_pool == nullptr; }

			Header<T>* operator->() { return m_pool; }
			const Header<T>* operator->() const { return m_pool; }

			Header<T>& operator*() {
				assert(m_pool);
				return *m_pool;
			}
			const Header<T>& operator*() const {
				assert(m_pool);
				return *m_pool;
			}
		};
	public:
		PoolAllocator(IRootAllocator& a) : IDerivedAllocator(a) {}

		void freeAll() {
			auto it = poolBegin();
			while (!it.isEnd()) {
				m_allocator.free(&*it++);
			}
		}

		bool reserve(index_t numBlocks) {
			if (numBlocks == 0 || numBlocks == INVALID_INDEX) return false;
			assert(sizeof(T) > sizeof(FreeBlock));

			size_t sHeader = align(sizeof(Header<T>));
			size_t sBuckets = align(numBlocks * sizeof(Bucket));
			size_t sBlocks = align(numBlocks * sizeof(T));
			size_t size = sHeader + sBuckets + sBlocks;
			void* ptr = m_allocator.allocate(size);
			if (!ptr) return false;

			word_t base = (word_t)ptr;
			auto pHeader = new (ptr) Header<T>(
				(Bucket*)(base + sHeader), // pBuckets
				(T*)(base + sHeader + sBuckets), // pBlocks
				numBlocks
			);

			initBuckets(pHeader->pBuckets, numBlocks);

			if (m_pStart == nullptr) {
				m_pStart = pHeader;
				m_pEnd = pHeader;
				m_pUsed = pHeader;
			}
			else {
				m_pEnd->next = pHeader;
				pHeader->prev = m_pEnd;
				m_pEnd = pHeader;
			}

			return true;
		}

		bool allocate(entity handle) {
			if (handle == entity::invalid()) return false;

			for (auto it = poolUsed(); !it.isEnd(); ++it) {
				if (allocate(*it, handle)) return true;
			}
			return false;
		}

		T* get(entity handle) {
			if (handle == entity::invalid()) return nullptr;

			for (auto it = poolBegin(); !it.isEnd(); ++it) {
				auto ptr = get(*it, handle);
				if (ptr) return ptr;
			}
			return nullptr;
		}

		bool free(entity handle) {
			if (handle == entity::invalid()) return false;

			for (auto it = poolBegin(); !it.isEnd(); ++it) {
				if (free(*it, handle)) return true;
			}
			return false;
		}
	private:
		Header<T>* m_pStart = nullptr;
		Header<T>* m_pEnd = nullptr;
		Header<T>* m_pUsed = nullptr;

		void initBuckets(Bucket* pBuckets, index_t length) {
			if (!length) return;

			int next = 1;
			int prev = INVALID_INDEX;
			for (index_t i = 0; i < length; ++i) {
				Bucket* b = new (pBuckets + i) Bucket();
				b->next = next++;
				b->prev = prev++;
			}

			index_t last = length - 1;
			pBuckets[0].prev = last;
			pBuckets[last].next = 0;
		}

		Iterator<T> poolBegin() {
			return Iterator<T>(*this, m_pStart);
		}

		Iterator<T> poolUsed() {
			return Iterator<T>(*this, m_pUsed);
		}

		inline index_t hash(Header<T>& pool, entity handle) {
			return (unsigned int)handle % pool.numBlocks;
		}

		inline Bucket& getBucket(Header<T>& pool, index_t index) {
			assert(index < pool.numBlocks);
			return pool.pBuckets[index];
		}

		Bucket& useBucket(Header<T>& pool, index_t index) {
			Bucket& b = getBucket(pool, index);
			assert(b.key == entity::invalid());
			assert(b.value == 0);
			Bucket& prev = getBucket(pool, b.prev);
			Bucket& next = getBucket(pool, b.next);
			prev.next = b.next;
			next.prev = b.prev;
			if (pool.freeBucket == index) {
				pool.freeBucket = index == b.next ? INVALID_INDEX : b.next;
			}
			return b;
		}

		void insertKey(Header<T>& pool, entity handle, index_t value) {
			index_t startIndex = hash(pool, handle);
			Bucket& start = getBucket(pool, startIndex);
			// if bucket not used
			if (start.key == entity::invalid()) {
				useBucket(pool, startIndex);
				start.set(handle, value, INVALID_INDEX, INVALID_INDEX);
			}
			// if bucket used by random key
			else if (start.key != handle) {
				Bucket& free = useBucket(pool, pool.freeBucket);
				free.set(start);
				start.set(handle, value, INVALID_INDEX, INVALID_INDEX);
			}
			// if bucket used by key with same hash
			else {
				index_t freeIndex = pool.freeBucket;
				Bucket& free = useBucket(pool, freeIndex);

				index_t nextIndex = start.next;
				if (nextIndex != INVALID_INDEX) {
					Bucket& next = getBucket(pool, nextIndex);
					next.prev = freeIndex;
				}

				start.next = freeIndex;
				free.set(handle, value, startIndex, nextIndex);
			}
		}

		inline bool validBlockIndex(Header<T>& pool, index_t index) {
			return pool.top <= pool.numBlocks && index < pool.top - 1;
		}

		inline T* getBlock(Header<T>& pool, index_t index) {
			assert(validBlockIndex(pool, index));
			return pool.pBlocks + index;
		}

		inline FreeBlock* getFreeBlock(Header<T>& pool, index_t index) {
			assert(validBlockIndex(pool, index));
			return (FreeBlock*)(pool.pBlocks + index);
		}

		index_t useFreeBlock(Header<T>& pool) {
			if (pool.freeBlock != INVALID_INDEX) {
				index_t freeIndex = pool.freeBlock;
				FreeBlock* free = getFreeBlock(pool, freeIndex);
				assert(free->prev == INVALID_INDEX);

				index_t nextIndex = free->next;
				if (nextIndex != INVALID_INDEX) {
					FreeBlock* next = getFreeBlock(pool, nextIndex);
					next->prev = INVALID_INDEX;
				}
				pool.freeBlock = nextIndex;

				return freeIndex;
			}
			else {
				assert(pool.top < pool.numBlocks);
				return pool.top++;
			}
		}

		bool allocate(Header<T>& pool, entity handle) {
			if (pool.top == pool.numBlocks && pool.freeBlock == INVALID_INDEX) return false;
			assert(pool.freeBucket != INVALID_INDEX);

			index_t index = useFreeBlock(pool);
			Bucket& b = getBucket(pool, index);

			assert(!b.used);
			b.used = true;

			insertKey(pool, handle, index);

			return true;
		}

		index_t getBucketIndex(Header<T>& pool, entity handle) {
			index_t index = hash(pool, handle);
			while (index != INVALID_INDEX) {
				Bucket& b = getBucket(pool, index);
				if (b.key == handle) {
					return index;
				}
				else {
					index = b.next;
				}
			}
			return INVALID_INDEX;
		}

		T* get(Header<T>& pool, entity handle) {
			index_t index = getBucketIndex(pool, handle);
			if (index == INVALID_INDEX) return nullptr;
			return pool.pBlocks + index;
		}

		index_t freeBucketFromHashChain(Header<T>& pool, index_t index) {
			Bucket& b = getBucket(pool, index);
			if (b.prev == INVALID_INDEX) {
				if (b.next == INVALID_INDEX) {
					return index;
				}
				else {
					index_t nextIndex = b.next;
					Bucket& next = getBucket(pool, nextIndex);
					next.prev = INVALID_INDEX;
					b.set(next);
					return nextIndex;
				}
			}
			else {
				index_t prevIndex = b.prev;
				Bucket& prev = getBucket(pool, prevIndex);
				index_t nextIndex = b.next;
				prev.next = nextIndex;
				if (nextIndex != INVALID_INDEX) {
					Bucket& next = getBucket(pool, nextIndex);
					next.prev = prevIndex;
				}
				return index;
			}
		}

		void addBucketToFreeChain(Header<T>& pool, index_t index) {
			Bucket& b = getBucket(pool, index);

			index_t startIndex = pool.freeBucket;
			if (startIndex == INVALID_INDEX) {
				startIndex = index;
				b.set(0, 0, index, index);
			}
			else {
				Bucket& start = getBucket(pool, startIndex);
				index_t nextIndex = start.next;
				Bucket& next = getBucket(pool, nextIndex);
				start.next = index;
				next.prev = index;
				b.set(0, 0, startIndex, nextIndex);
			}
		}

		void freeBlockFromFreeChain(Header<T>& pool, index_t index) {
			FreeBlock& block = *getFreeBlock(pool, index);
			if (block.prev == INVALID_INDEX) {
				assert(pool.freeBlock == index);
				if (block.next == INVALID_INDEX) {
					pool.freeBlock = INVALID_INDEX;
				}
				else {
					index_t nextIndex = block.next;
					FreeBlock& next = *getFreeBlock(pool, nextIndex);
					next.prev = INVALID_INDEX;
					pool.freeBlock = nextIndex;
				}
			}
			else {
				index_t prevIndex = block.prev;
				FreeBlock& prev = *getFreeBlock(pool, prevIndex);
				index_t nextIndex = block.next;
				prev.next = nextIndex;
				if (nextIndex != INVALID_INDEX) {
					FreeBlock& next = *getFreeBlock(pool, nextIndex);
					next.prev = prevIndex;
				}
			}
		}

		void freeBlock(Header<T>& pool, index_t index) {
			if (pool.top == index + 1) {
				index_t neighborIndex = index - 1;
				Bucket& b = getBucket(pool, neighborIndex);
				if (!b.used) {
					freeBlockFromFreeChain(pool, neighborIndex);
				}
				pool.top--;
			}
			else {
				freeBlockFromFreeChain(pool, index);
			}
		}

		bool free(Header<T>& pool, entity handle) {
			index_t bucketIndex = getBucketIndex(pool, handle);
			if (bucketIndex == INVALID_INDEX) return false;

			Bucket& bucket = getBucket(pool, bucketIndex);
			index_t blockIndex = bucket.value;
			Bucket& usedBucket = getBucket(pool, blockIndex);
			assert(usedBucket.used);
			usedBucket.used = false;

			index_t freeIndex = freeBucketFromHashChain(pool, bucketIndex);
			addBucketToFreeChain(pool, freeIndex);

			freeBlock(pool, blockIndex);

			return true;
		}
	};
}}
#endif
