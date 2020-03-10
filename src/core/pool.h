
#ifndef POOL_H
#define POOL_H

#include <variant>
#include <iostream>

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
			assert(m_entity != entity::invalid());
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

			T* get(entity handle) {
				index_t index = getBucketIndex(handle);
				if (index == INVALID_INDEX) return nullptr;
				Bucket& b = getBucket(index);
				T* ptr = getBlockAs<T>(b.value);
				assert(ptr);
				return ptr;
			}

			bool allocate(entity handle) {
				if (this->top == this->numBlocks &&
					this->freeBlockIndex == INVALID_INDEX) return false;
				if (getBucketIndex(handle) != INVALID_INDEX) return false;
				assert(this->freeBucketIndex != INVALID_INDEX);

				index_t blockIndex = useBlock();
				*getBlock(blockIndex) = T();
				insertKey(handle, blockIndex);

				return true;
			}

			bool free(entity handle) {
				index_t bucketIndex = getBucketIndex(handle);
				if (bucketIndex == INVALID_INDEX) return false;

				index_t blockIndex = getBucket(bucketIndex).value;
				freeBlock(blockIndex);

				index_t freeIndex = freeBucketFromHashChain(bucketIndex);
				linkBucket(freeIndex);

				return true;
			}

			unsigned int getType(entity handle) {
				index_t index = getBucketIndex(handle);
				if (index == INVALID_INDEX) return -1;
				Bucket& b = getBucket(index);
				Block& block = *getBlock(b.value);
				return block.index();
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

			Bucket& useBucket(index_t freeIndex) {
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

			void insertKey(entity handle, index_t value) {
				index_t startIndex = hash(handle);
				Bucket& start = getBucket(startIndex);

				if (start.key == entity::invalid()) {
					// if bucket not used, use.
					useBucket(startIndex);
					start = { handle, value };
				}
				else if (hash(start.key) == startIndex) {
					/* if bucket used by key with same hash,
					   insert key into a new bucket between
					   start and next. */

					index_t freeIndex = this->freeBucketIndex;
					Bucket& free = useBucket(freeIndex);

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
					Bucket& free = useBucket(freeIndex);
					free = start;

					if (free.prev != INVALID_INDEX) {
						Bucket& prev = getBucket(free.prev);
						prev.next = freeIndex;
					}
					if (free.next != INVALID_INDEX) {
						Bucket& next = getBucket(free.next);
						next.prev = freeIndex;
					}

					start = { handle, value };
				}
			}

			inline Block* getBlock(index_t index) {
				assert(index < this->numBlocks);
				return this->pBlocks + index;
			}

			template<typename U>
			inline U* getBlockAs(index_t index) {
				return std::get_if<U>(getBlock(index));
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
					   use the starting block. */

					index_t freeIndex = this->freeBlockIndex;
					LinkedBlock& free = getBlockRefAs<LinkedBlock>(freeIndex);
					assert(free.prev == INVALID_INDEX);

					// set next block in chain as the new start
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
				if (this->top == index + 1) {
					/* if the topmost block is
					   part of the free chain. */

					this->top--;

					Block& topBlock = *getBlock(index);
					assert(std::holds_alternative<T>(topBlock));
					topBlock = FreeBlock();

					for (index_t prevIndex = index; prevIndex-- > 0;) {
						Block& prevBlock = *getBlock(prevIndex);
						if (!std::holds_alternative<LinkedBlock>(prevBlock)) break;

						unlinkBlock(prevIndex);
						prevBlock = FreeBlock();
						this->top--;
					}
				}
				else {
					addBlockToFreeChain(index);
				}
			}

			void addBlockToFreeChain(index_t freeIndex) {
				Block& freeBlock = *getBlock(freeIndex);
				assert(!std::holds_alternative<LinkedBlock>(freeBlock));

				freeBlock = LinkedBlock();
				LinkedBlock& free = getBlockRefAs<LinkedBlock>(freeIndex);

				if (this->freeBlockIndex == INVALID_INDEX) {
					this->freeBlockIndex = freeIndex;
				}
				else {
					index_t startIndex = this->freeBlockIndex;
					LinkedBlock& start = getBlockRefAs<LinkedBlock>(startIndex);
					
					index_t nextIndex = start.next;
					if (nextIndex != INVALID_INDEX) {
						LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
						next.prev = freeIndex;
					}
					start.next = freeIndex;

					free.prev = startIndex;
					free.next = nextIndex;
				}
			}

			void unlinkBlock(index_t index) {
				LinkedBlock& block = getBlockRefAs<LinkedBlock>(index);

				if (block.prev == INVALID_INDEX) {
					assert(this->freeBlockIndex == index);

					if (block.next == INVALID_INDEX) {
						this->freeBlockIndex = INVALID_INDEX;
					}
					else {
						index_t nextIndex = block.next;
						LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
						next.prev = INVALID_INDEX;
						this->freeBlockIndex = nextIndex;
					}
				}
				else {
					index_t prevIndex = block.prev;
					LinkedBlock& prev = getBlockRefAs<LinkedBlock>(prevIndex);

					index_t nextIndex = block.next;
					if (nextIndex != INVALID_INDEX) {
						LinkedBlock& next = getBlockRefAs<LinkedBlock>(nextIndex);
						next.prev = prevIndex;
					}
					prev.next = nextIndex;
				}
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

			index_t freeBucketFromHashChain(index_t startIndex) {
				Bucket& start = getBucket(startIndex);
				if (start.prev == INVALID_INDEX) {
					if (start.next == INVALID_INDEX) {
						return startIndex;
					}
					else {
						index_t nextIndex = start.next;
						Bucket& next = getBucket(nextIndex);
						next.prev = INVALID_INDEX;
						start = next;
						return nextIndex;
					}
				}
				else {
					index_t prevIndex = start.prev;
					Bucket& prev = getBucket(prevIndex);

					index_t nextIndex = start.next;
					if (nextIndex != INVALID_INDEX) {
						Bucket& next = getBucket(nextIndex);
						next.prev = prevIndex;
					}
					prev.next = nextIndex;

					return startIndex;
				}
			}

			void linkBucket(index_t freeIndex) {
				Bucket& free = getBucket(freeIndex);
				index_t startIndex = this->freeBucketIndex;

				if (startIndex == INVALID_INDEX) {
					this->freeBucketIndex = freeIndex;
					Bucket temp{ entity::invalid(), 0, freeIndex, freeIndex };
					free = temp;
				}
				else {
					Bucket& start = getBucket(startIndex);
					index_t nextIndex = start.next;
					Bucket& next = getBucket(nextIndex);

					start.next = freeIndex;
					next.prev = freeIndex;

					Bucket temp{ entity::invalid(), 0, startIndex, nextIndex };
					free = temp;
				}
			}
		};

		class Iterator {
		private:
			PoolAllocator& m_allocator;
			Pool* m_pool = nullptr;
		public:
			Iterator(PoolAllocator& a, Pool* pool)
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
		PoolAllocator(IRootAllocator& a) : IDerivedAllocator(a) {}

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
				(Bucket*)(base + sHeader), // pBuckets
				(Block*)(base + sHeader + sBuckets) // pBlocks
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

		T* get(entity handle) {
			if (handle == entity::invalid()) return nullptr;

			for (auto it = poolBegin(); !it.isEnd(); ++it) {
				auto ptr = it->get(handle);
				if (ptr) return ptr;
			}
			return nullptr;
		}

		unsigned int getType(entity handle) {
			return poolBegin()->getType(handle);
		}

		bool free(entity handle) {
			if (handle == entity::invalid()) return false;

			for (auto it = poolBegin(); !it.isEnd(); ++it) {
				if (it->free(handle)) return true;
			}
			return false;
		}

		void clear() {
			for (auto it = poolBegin(); !it.isEnd(); it++) {
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
}}
#endif
