
#ifndef CORE_COMPONENT_POOL_H
#define CORE_COMPONENT_POOL_H

#include "component_allocator.h"
#include "component_iterator.h"
#include "entity.h"
#include "utility/boolean.h"
#include <variant>
#include <cassert>

namespace TEngine::Core {

template<class T>
class PooledComponentAllocator : public IComponentAllocator<T> {
private:
	using index_t = unsigned short;
	constexpr static index_t INVALID_INDEX = (index_t)-1;

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
		friend class PooledComponentIterator;
	public:
		Pool* next = nullptr;
		Pool* prev = nullptr;
	private:
		Bucket* m_buckets = nullptr;
		Block* m_blocks = nullptr;
		index_t m_freeBucketIndex = 0;
		index_t m_freeBlockIndex = INVALID_INDEX;
		index_t m_top = 0;
		index_t m_numBlocks = 0;
	public:
		Pool(index_t n, Bucket* buckets, Block* blocks)
			: m_numBlocks(n), m_buckets(buckets), m_blocks(blocks) {
			initBuckets();
			initBlocks();
		}

		bool has(entity handle) const {
			index_t index = getBucketIndex(handle);
			return index != INVALID_INDEX;
		}

		bool has(index_t index) const {
			return get(index);
		}

		T* get(entity handle) const {
			index_t index = getBucketIndex(handle);
			if (index == INVALID_INDEX) return nullptr;

			Bucket& b = getBucket(index);
			return getBlockAs<T>(b.value);
		}

		T* get(index_t index) const {
			if (index == INVALID_INDEX) return nullptr;
			return getBlockAs<T>(index);
		}

		bool allocate(entity handle) {
			// if pool is full, return false
			if (this->m_top == this->m_numBlocks &&
				this->m_freeBlockIndex == INVALID_INDEX) return false;
			// if handle is already registered, return false
			if (getBucketIndex(handle) != INVALID_INDEX) return false;
			assert(this->m_freeBucketIndex != INVALID_INDEX);

			index_t blockIndex = useBlock();
			getBlock(blockIndex).emplace<T>(handle);
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
			for (index_t i = 0; i < this->m_numBlocks; ++i) {
				Bucket* b = new (this->m_buckets + i) Bucket();
				b->next = next++;
				b->prev = prev++;
			}

			// loop start and end links
			index_t last = this->m_numBlocks - 1;
			this->m_buckets[0].prev = last;
			this->m_buckets[last].next = 0;
		}

		void initBlocks() {
			for (index_t i = 0; i < this->m_numBlocks; ++i) {
				Block* b = new (this->m_blocks + i) Block();
			}
		}

		index_t hash(entity handle) const {
			return (unsigned int)handle % this->m_numBlocks;
		}

		Bucket& getBucket(index_t index) const {
			assert(index < this->m_numBlocks);
			return this->m_buckets[index];
		}

		index_t getBucketIndex(entity handle) const {
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
			if (this->m_freeBucketIndex == freeIndex) {
				bool lastBucket = freeIndex == free.next;
				this->m_freeBucketIndex = lastBucket ? INVALID_INDEX : free.next;
			}
			return free;
		}

		void linkBucket(index_t freeIndex) {
			Bucket& free = getBucket(freeIndex);
			index_t startIndex = this->m_freeBucketIndex;

			if (startIndex == INVALID_INDEX) {
				this->m_freeBucketIndex = freeIndex;
				free = Bucket{ entity::invalid(), 0, freeIndex, freeIndex };
			}
			else {
				Bucket& run = getBucket(startIndex);
				index_t nextIndex = run.next;
				Bucket& next = getBucket(nextIndex);

				run.next = freeIndex;
				next.prev = freeIndex;

				free = Bucket{ entity::invalid(), 0, startIndex, nextIndex };
			}
		}

		void useBucket(entity handle, index_t value) {
			index_t startIndex = hash(handle);
			Bucket& run = getBucket(startIndex);

			if (run.key == entity::invalid()) {
				unlinkBucket(startIndex);
				run = { handle, value };
			}
			else if (hash(run.key) == startIndex) {
				/* if bucket used by key with same hash,
					insert key into a new bucket between
					start and next. */

				index_t freeIndex = this->m_freeBucketIndex;
				Bucket& free = unlinkBucket(freeIndex);

				index_t nextIndex = run.next;
				if (nextIndex != INVALID_INDEX) {
					Bucket& next = getBucket(nextIndex);
					next.prev = freeIndex;
				}
				run.next = freeIndex;

				free = { handle, value, startIndex, nextIndex };
			}
			else {
				/* if bucket used by random key,
					move random key to a free bucket
					and insert key into this bucket. */

				index_t freeIndex = this->m_freeBucketIndex;
				Bucket& free = unlinkBucket(freeIndex);

				free = run;
				run = { handle, value };

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

		Block& getBlock(index_t index) const {
			assert(index < this->m_numBlocks);
			return this->m_blocks[index];
		}

		template<typename U>
		U* getBlockAs(index_t index) const {
			return std::get_if<U>(&getBlock(index));
		}

		template<typename U>
		U& getBlockRefAs(index_t index) const {
			U* ptr = getBlockAs<U>(index);
			assert(ptr);
			return *ptr;
		}

		index_t useBlock() {
			if (this->m_freeBlockIndex != INVALID_INDEX) {
				/* if free list exists,
					use the first block. */

				index_t freeIndex = this->m_freeBlockIndex;
				LinkedBlock& free = getBlockRefAs<LinkedBlock>(freeIndex);
				assert(free.prev == INVALID_INDEX);

				// set the next block in the chain as the new head
				index_t nextIndex = free.next;
				if (nextIndex != INVALID_INDEX) {
					LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
					next.prev = INVALID_INDEX;
				}
				this->m_freeBlockIndex = nextIndex;

				return freeIndex;
			}
			else {
				/* get block from top of the stack */
				assert(this->m_top < this->m_numBlocks);
				return this->m_top++;
			}
		}

		void freeBlock(index_t index) {
			if (index < this->m_top - 1) {
				linkBlock(index);
			}
			else {
				freeTopBlock(index);
			}
		}

		void freeTopBlock(index_t index) {
			this->m_top--;

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
				this->m_top--;
			}
		}

		void linkBlock(index_t curIndex) {
			Block& block = getBlock(curIndex);
			assert(!std::holds_alternative<LinkedBlock>(block));
			block = LinkedBlock();
			LinkedBlock& cur = getBlockRefAs<LinkedBlock>(curIndex);

			if (this->m_freeBlockIndex == INVALID_INDEX) {
				this->m_freeBlockIndex = curIndex;
			}
			else {
				index_t startIndex = this->m_freeBlockIndex;
				LinkedBlock& run = getBlockRefAs<LinkedBlock>(startIndex);
					
				index_t nextIndex = run.next;
				if (nextIndex != INVALID_INDEX) {
					LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
					next.prev = curIndex;
				}
				run.next = curIndex;

				cur.prev = startIndex;
				cur.next = nextIndex;
			}
		}

		void unlinkBlock(index_t curIndex) {
			LinkedBlock& cur = getBlockRefAs<LinkedBlock>(curIndex);

			// if cur is the head of the chain
			if (cur.prev == INVALID_INDEX) {
				assert(this->m_freeBlockIndex == curIndex);

				// if the chain ends at head
				if (cur.next == INVALID_INDEX) {
					this->m_freeBlockIndex = INVALID_INDEX;
				}
				else {
					// set next as the new head
					index_t nextIndex = cur.next;
					LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
					next.prev = INVALID_INDEX;
					this->m_freeBlockIndex = nextIndex;
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

	class PoolIterator {
	private:
		const PooledComponentAllocator& m_allocator;
		Pool* m_pool = nullptr;
	public:
		PoolIterator(const PooledComponentAllocator& allocator)
			: m_allocator(allocator) { }

		PoolIterator(const PooledComponentAllocator& allocator, Pool* pool)
			: m_allocator(allocator), m_pool(pool) { }

		PoolIterator& operator++() {
			if (!m_pool) {
				throw "Invalid State. PoolIterator cannot increment.";
			}

			if (m_pool == m_allocator.m_poolEnd) {
				m_pool = nullptr;
			}
			else {
				auto ptr = m_pool->next;
				assert(ptr);
				m_pool = ptr;
			}
			return *this;
		}

		PoolIterator& operator--() {
			if (m_pool == m_allocator.m_poolStart) {
				throw "Invalid State. PoolIterator cannot decrement.";
			}

			if (m_pool == nullptr) {
				m_pool = m_allocator.m_poolEnd;
			}
			else {
				auto ptr = m_pool->prev;
				assert(ptr);
				m_pool = ptr;
			}
			return *this;
		}

		explicit operator bool() const { return m_pool; }

		const Pool* operator->() const { return m_pool; }
		Pool* operator->() { return m_pool; }

		const Pool& operator*() const {
			if (m_pool) return *m_pool;
			else throw "PoolIterator could not dereference pointer.";
		}
		Pool& operator*() {
			return const_cast<Pool&>(static_cast<const PoolIterator&>(*this).operator*());
		}
	};
public:
	class PooledComponentIterator : public IComponentIterator<T> {
	private:
		PooledComponentAllocator& m_allocator;
		PoolIterator m_poolIt;
		index_t m_index = INVALID_INDEX;
	public:
		PooledComponentIterator(
			PooledComponentAllocator& allocator, PoolIterator poolIt, index_t index)
			: m_allocator(allocator), m_poolIt(poolIt), m_index(index) {
			if (m_poolIt) operator++();
		}

		explicit operator bool() const override { return (bool)m_poolIt; }
		entity getEntity() const override {
			auto ptr = operator->();
			if (ptr) return ptr->entityId;
			else return entity::invalid();
		}

		PooledComponentIterator& operator++() override {
			if (!m_poolIt) throw "Invalid Call. Iterator object has finished.";

			while (true) {
				if (!m_poolIt) {
					break;
				}
				else if (m_index == m_poolIt->m_top) {
					assert(m_poolIt->m_top != INVALID_INDEX);
					++m_poolIt;
					m_index = INVALID_INDEX;
				}
				else {
					++m_index;
					if (m_poolIt->has(m_index)) break;
				}
			}
			return *this;
		}

		const T* operator->() const override {
			if (m_poolIt) return m_poolIt->get(m_index);
			else return nullptr;
		}
		T* operator->() override {
			return const_cast<T*>(static_cast<const PooledComponentIterator&>(*this).operator->());
		}

		const T& operator*() const override {
			const T* ptr = operator->();
			if (ptr) return *ptr;
			else throw "PooledComponentIterator could not dereference pointer.";
		}
		T& operator*() override {
			return const_cast<T&>(static_cast<const PooledComponentIterator&>(*this).operator*());
		}
	};
private:
	using IComponentAllocator<T>::m_allocator;
	using IComponentAllocator<T>::m_count;

	Pool* m_poolStart = nullptr;
	Pool* m_poolEnd = nullptr;
	bool m_running = true;
public:
	using IComponentAllocator<T>::IComponentAllocator;

	bool reserve(index_t m_numBlocks) {
		assert(m_running);
		if (!m_running) return false;
		if (m_numBlocks == 0 || m_numBlocks == INVALID_INDEX) return false;

		size_t sHeader = align(sizeof(Pool));
		size_t sBuckets = align(m_numBlocks * sizeof(Bucket));
		size_t sBlocks = align(m_numBlocks * sizeof(Block));
		size_t size = sHeader + sBuckets + sBlocks;
		void* ptr = m_allocator.allocate(size);
		if (!ptr) return false;

		word_t base = (word_t)ptr;
		auto pHeader = new (ptr) Pool(
			m_numBlocks,
			(Bucket*)(base + sHeader),
			(Block*)(base + sHeader + sBuckets)
		);

		if (m_poolStart == nullptr) {
			m_poolStart = pHeader;
			m_poolEnd = pHeader;
		}
		else {
			m_poolEnd->next = pHeader;
			pHeader->prev = m_poolEnd;
			m_poolEnd = pHeader;
		}

		return true;
	}

	bool has(entity handle) const override {
		assert(m_running);
		if (!m_running) return false;
		if (handle == entity::invalid()) return false;

		for (auto it = poolBegin(); it; ++it) {
			if (it->has(handle)) return true;
		}
		return false;
	}

	T* get(entity handle) const override {
		assert(m_running);
		if (!m_running) return nullptr;
		if (handle == entity::invalid()) return nullptr;

		for (auto it = poolBegin(); it; ++it) {
			auto ptr = it->get(handle);
			if (ptr) return ptr;
		}
		return nullptr;
	}

	bool allocate(entity handle) override {
		assert(m_running);
		if (!m_running) return false;
		if (handle == entity::invalid()) return false;

		for (auto it = poolBegin(); it; ++it) {
			if (it->allocate(handle)) {
				m_count++;
				return true;
			}
		}
		return false;
	}

	bool free(entity handle) override {
		assert(m_running);
		if (!m_running) return false;
		if (handle == entity::invalid()) return false;

		for (auto it = poolBegin(); it; ++it) {
			if (it->free(handle)) {
				m_count--;
				return true;
			}
		}
		return false;
	}

	void shutDown() {
		if (!Utility::toggle<false>(m_running)) return;

		for (auto it = poolBegin(); it; ++it) {
			Pool* pool = &*it;
			pool->~Pool();
			m_allocator.free(pool);
		}
		m_count = 0;
	}

	PooledComponentIterator begin() {
		return PooledComponentIterator(*this, poolBegin(), INVALID_INDEX);
	}
private:
	PoolIterator poolBegin() const {
		return PoolIterator(*this, m_poolStart);
	}
};
}
#endif
