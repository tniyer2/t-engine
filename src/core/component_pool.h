
#ifndef CORE_COMPONENT_POOL_H
#define CORE_COMPONENT_POOL_H

#include "component.h"
#include <variant>

namespace TEngine::Core {
using index_t = unsigned short;
static constexpr index_t INVALID_INDEX = (index_t)-1;

template<class T>
class PooledComponentAllocator : public IComponentAllocator<T> {
	using IComponentAllocator<T>::m_allocator;

	struct FreeBlock {};
	struct LinkedBlock {
		index_t next = INVALID_INDEX;
		index_t prev = INVALID_INDEX;
	};
	using Block = std::variant<FreeBlock, LinkedBlock, T>;

	struct Bucket {
		entity key;
		index_t value = 0;
		index_t prev = INVALID_INDEX;
		index_t next = INVALID_INDEX;
	};

	class Pool {
	public:
		Pool* next = nullptr;
		Pool* prev = nullptr;
	private:
		Bucket* pBuckets = nullptr;
		Block* pBlocks = nullptr;
		index_t freeBucketIndex = 0;
		index_t freeBlockIndex = INVALID_INDEX;
		index_t top = 0;
		index_t numBlocks = 0;
	public:
		Pool(index_t n, Bucket* buckets, Block* blocks)
			: numBlocks(n), pBuckets(buckets), pBlocks(blocks) {
			initBuckets();
			initBlocks();
		}
		~Pool() {}

		bool has(entity handle) {
			index_t index = getBucketIndex(handle);
			return index != INVALID_INDEX;
		}

		T* get(entity handle) {
			index_t index = getBucketIndex(handle);
			if (index == INVALID_INDEX) return nullptr;

			Bucket& b = getBucket(index);
			T* ptr = getBlockAs<T>(b.value);
			assert(ptr);
			return ptr;
		}

		bool allocate(entity handle) {
			// if pool is full, return false
			if (this->top == this->numBlocks &&
				this->freeBlockIndex == INVALID_INDEX) return false;
			// if handle is already registered, return false
			if (getBucketIndex(handle) != INVALID_INDEX) return false;
			assert(this->freeBucketIndex != INVALID_INDEX);

			index_t blockIndex = useBlock();
			getBlock(blockIndex) = T();
			useBucket(handle, blockIndex);

			return true;
		}

		bool free(entity handle) {
			index_t bucketIndex = getBucketIndex(handle);
			if (bucketIndex == INVALID_INDEX) return false;

			Bucket& bucket = getBucket(bucketIndex);
			freeBlock(bucket.value);

			index_t freeIndex = unlinkBucketFromHashChain(bucketIndex);
			linkBucket(freeIndex);

			return true;
		}
	private:
		void initBuckets() {
			// create and link buckets to adjacent buckets
			index_t next = 1;
			index_t prev = INVALID_INDEX;
			for (index_t i = 0; i < this->numBlocks; ++i) {
				Bucket* b = new (this->pBuckets + i) Bucket();
				b->next = next++;
				b->prev = prev++;
			}

			// loop start and end links
			index_t last = this->numBlocks - 1;
			this->pBuckets[0].prev = last;
			this->pBuckets[last].next = 0;
		}

		void initBlocks() {
			for (index_t i = 0; i < this->numBlocks; ++i) {
				Block* b = new (this->pBlocks + i) Block();
			}
		}

		inline index_t hash(entity handle) {
			return (unsigned int)handle % this->numBlocks;
		}

		inline Bucket& getBucket(index_t index) {
			assert(index < this->numBlocks);
			return this->pBuckets[index];
		}

		index_t getBucketIndex(entity handle) {
			index_t index = hash(handle);

			while (index != INVALID_INDEX) {
				Bucket& b = getBucket(index);
				if (b.key == entity::invalid()) {
					return INVALID_INDEX;
				}
				else if (b.key == handle) {
					return index;
				}
				else {
					index = b.next;
				}
			}
			return INVALID_INDEX;
		}

		Bucket& unlinkBucket(index_t freeIndex) {
			Bucket& free = getBucket(freeIndex);
			assert(free.key == entity::invalid());
			assert(free.value == 0);
			assert(free.prev != INVALID_INDEX);
			assert(free.next != INVALID_INDEX);

			// join sibling nodes
			Bucket& prev = getBucket(free.prev);
			Bucket& next = getBucket(free.next);
			prev.next = free.next;
			next.prev = free.prev;

			// update free index
			if (this->freeBucketIndex == freeIndex) {
				bool lastBucket = freeIndex == free.next;
				this->freeBucketIndex = lastBucket ? INVALID_INDEX : free.next;
			}
			return free;
		}

		void linkBucket(index_t freeIndex) {
			Bucket& free = getBucket(freeIndex);
			index_t startIndex = this->freeBucketIndex;

			if (startIndex == INVALID_INDEX) {
				this->freeBucketIndex = freeIndex;
				free = Bucket{ entity::invalid(), 0, freeIndex, freeIndex };
			}
			else {
				Bucket& start = getBucket(startIndex);
				index_t nextIndex = start.next;
				Bucket& next = getBucket(nextIndex);

				start.next = freeIndex;
				next.prev = freeIndex;

				free = Bucket{ entity::invalid(), 0, startIndex, nextIndex };
			}
		}

		void useBucket(entity handle, index_t value) {
			index_t startIndex = hash(handle);
			Bucket& start = getBucket(startIndex);

			if (start.key == entity::invalid()) {
				unlinkBucket(startIndex);
				start = { handle, value };
			}
			else if (hash(start.key) == startIndex) {
				/* if bucket used by key with same hash,
					insert key into a new bucket between
					start and next. */

				index_t freeIndex = this->freeBucketIndex;
				Bucket& free = unlinkBucket(freeIndex);

				index_t nextIndex = start.next;
				if (nextIndex != INVALID_INDEX) {
					Bucket& next = getBucket(nextIndex);
					next.prev = freeIndex;
				}
				start.next = freeIndex;

				free = { handle, value, startIndex, nextIndex };
			}
			else {
				/* if bucket used by random key,
					move random key to a free bucket
					and insert key into this bucket. */

				index_t freeIndex = this->freeBucketIndex;
				Bucket& free = unlinkBucket(freeIndex);

				free = start;
				start = { handle, value };

				if (free.prev != INVALID_INDEX) {
					Bucket& prev = getBucket(free.prev);
					prev.next = freeIndex;
				}
				if (free.next != INVALID_INDEX) {
					Bucket& next = getBucket(free.next);
					next.prev = freeIndex;
				}
			}
		}

		index_t unlinkBucketFromHashChain(index_t curIndex) {
			Bucket& cur = getBucket(curIndex);

			// if head of chain
			if (cur.prev == INVALID_INDEX) {
				// if chain ends at head
				if (cur.next == INVALID_INDEX) {
					// free head
					return curIndex;
				}
				else {
					// move next to head and free next
					index_t nextIndex = cur.next;
					Bucket& next = getBucket(nextIndex);
					cur = next;
					cur.prev = INVALID_INDEX;
					return nextIndex;
				}
			}
			else {
				// free this bucket and join prev and next

				index_t prevIndex = cur.prev;
				Bucket& prev = getBucket(prevIndex);

				index_t nextIndex = cur.next;
				if (nextIndex != INVALID_INDEX) {
					Bucket& next = getBucket(nextIndex);
					next.prev = prevIndex;
				}
				prev.next = nextIndex;

				return curIndex;
			}
		}

		inline Block& getBlock(index_t index) {
			assert(index < this->numBlocks);
			return this->pBlocks[index];
		}

		template<typename U>
		inline U* getBlockAs(index_t index) {
			return std::get_if<U>(&getBlock(index));
		}

		template<typename U>
		inline U& getBlockRefAs(index_t index) {
			U* ptr = getBlockAs<U>(index);
			assert(ptr);
			return *ptr;
		}

		index_t useBlock() {
			if (this->freeBlockIndex != INVALID_INDEX) {
				/* if free list exists,
					use the first block. */

				index_t freeIndex = this->freeBlockIndex;
				LinkedBlock& free = getBlockRefAs<LinkedBlock>(freeIndex);
				assert(free.prev == INVALID_INDEX);

				// set the next block in the chain as the new head
				index_t nextIndex = free.next;
				if (nextIndex != INVALID_INDEX) {
					LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
					next.prev = INVALID_INDEX;
				}
				this->freeBlockIndex = nextIndex;

				return freeIndex;
			}
			else {
				/* get block from top of the stack */
				assert(this->top < this->numBlocks);
				return this->top++;
			}
		}

		void freeBlock(index_t index) {
			if (index < this->top - 1) {
				linkBlock(index);
			}
			else {
				freeTopBlock(index);
			}
		}

		void freeTopBlock(index_t index) {
			this->top--;

			Block& topBlock = getBlock(index);
			assert(std::holds_alternative<T>(topBlock));
			topBlock = FreeBlock();

			/* recursively free all of the
				linked blocks directly below
				the top block */
			for (index_t prevIndex = index; prevIndex-- > 0;) {
				Block& prevBlock = getBlock(prevIndex);
				if (!std::holds_alternative<LinkedBlock>(prevBlock)) break;

				unlinkBlock(prevIndex);
				prevBlock = FreeBlock();
				this->top--;
			}
		}

		void linkBlock(index_t curIndex) {
			Block& block = getBlock(curIndex);
			assert(!std::holds_alternative<LinkedBlock>(block));
			block = LinkedBlock();
			LinkedBlock& cur = getBlockRefAs<LinkedBlock>(curIndex);

			if (this->freeBlockIndex == INVALID_INDEX) {
				this->freeBlockIndex = curIndex;
			}
			else {
				index_t startIndex = this->freeBlockIndex;
				LinkedBlock& start = getBlockRefAs<LinkedBlock>(startIndex);
					
				index_t nextIndex = start.next;
				if (nextIndex != INVALID_INDEX) {
					LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
					next.prev = curIndex;
				}
				start.next = curIndex;

				cur.prev = startIndex;
				cur.next = nextIndex;
			}
		}

		void unlinkBlock(index_t curIndex) {
			LinkedBlock& cur = getBlockRefAs<LinkedBlock>(curIndex);

			// if cur is the head of the chain
			if (cur.prev == INVALID_INDEX) {
				assert(this->freeBlockIndex == curIndex);

				// if the chain ends at head
				if (cur.next == INVALID_INDEX) {
					this->freeBlockIndex = INVALID_INDEX;
				}
				else {
					// set next as the new head
					index_t nextIndex = cur.next;
					LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
					next.prev = INVALID_INDEX;
					this->freeBlockIndex = nextIndex;
				}
			}
			else {
				index_t prevIndex = cur.prev;
				LinkedBlock& prev = getBlockRefAs<LinkedBlock>(prevIndex);

				index_t nextIndex = cur.next;
				if (nextIndex != INVALID_INDEX) {
					LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
					next.prev = prevIndex;
				}
				prev.next = nextIndex;
			}
		}
	};

	class Iterator {
	private:
		PooledComponentAllocator& m_allocator;
		Pool* m_pool = nullptr;
	public:
		Iterator(PooledComponentAllocator& a, Pool* pool)
			: m_allocator(a), m_pool(pool) {}

		Iterator& operator++() {
			assert(m_pool != nullptr);
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
		Iterator operator++(int) {
			Iterator temp(*this);
			operator++();
			return temp;
		}

		Iterator& operator--() {
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
		Iterator operator--(int) {
			Iterator temp(*this);
			operator--();
			return temp;
		}

		bool isEnd() { return !m_pool; }
		bool isEnd() const { return !m_pool; }

		Pool* operator->() {
			assert(m_pool);
			return m_pool;
		}
		const Pool* operator->() const {
			assert(m_pool);
			return m_pool;
		}

		Pool& operator*() {
			assert(m_pool);
			return *m_pool;
		}
		const Pool& operator*() const {
			assert(m_pool);
			return *m_pool;
		}
	};
public:
	PooledComponentAllocator(IRootAllocator& a) : IComponentAllocator<T>(a) { }

	bool reserve(index_t numBlocks) {
		if (numBlocks == 0 || numBlocks == INVALID_INDEX) return false;

		size_t sHeader = align(sizeof(Pool));
		size_t sBuckets = align(numBlocks * sizeof(Bucket));
		size_t sBlocks = align(numBlocks * sizeof(Block));
		size_t size = sHeader + sBuckets + sBlocks;
		void* ptr = m_allocator.allocate(size);
		if (!ptr) return false;

		word_t base = (word_t)ptr;
		auto pHeader = new (ptr) Pool(
			numBlocks,
			(Bucket*)(base + sHeader),
			(Block*)(base + sHeader + sBuckets)
		);

		if (m_pStart == nullptr) {
			m_pStart = pHeader;
			m_pEnd = pHeader;
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

		for (auto it = poolBegin(); !it.isEnd(); ++it) {
			if (it->allocate(handle)) return true;
		}
		return false;
	}

	bool has(entity handle) {
		if (handle == entity::invalid()) return false;

		for (auto it = poolBegin(); !it.isEnd(); ++it) {
			if (it->has(handle)) return true;
		}
		return false;
	}

	T* get(entity handle) {
		if (handle == entity::invalid()) return nullptr;

		for (auto it = poolBegin(); !it.isEnd(); ++it) {
			auto ptr = it->get(handle);
			if (ptr) return ptr;
		}
		return nullptr;
	}

	bool free(entity handle) {
		if (handle == entity::invalid()) return false;

		for (auto it = poolBegin(); !it.isEnd(); ++it) {
			if (it->free(handle)) return true;
		}
		return false;
	}

	void shutDown() {
		for (auto it = poolBegin(); !it.isEnd(); ++it) {
			Pool* pool = &*it;
			pool->~Pool();
			m_allocator.free(pool);
		}
	}
private:
	Pool* m_pStart = nullptr;
	Pool* m_pEnd = nullptr;

	Iterator poolBegin() {
		return Iterator(*this, m_pStart);
	}
};
}
#endif