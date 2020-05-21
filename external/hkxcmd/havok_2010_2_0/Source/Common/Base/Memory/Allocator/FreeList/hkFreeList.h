/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FREELIST
#define HK_FREELIST

/// Fast fixed memory size allocations - used in the hkFreeListAllocator for small memory block allocations.
///
/// Can be used where ever you want fast same block size allocations, and you want to be able to free blocks
/// in any order when they are not in use.
///
/// This implementation is particularly fast at initial allocation - as it works just like a 'region'.
/// If memory is allocated and freed, the freelist uses a linked list of available blocks which whilst fast isn't
/// as fast as region style allocation. Doing a findGarbage will turn all memory that can be used as a region
/// into a region.
///
/// NOTE! Freeing freelist blocks does not free memory back to the allocator. Memory is only given back to the
/// hkFreeListAllocator when a garbageCollect or freeAllFreeBlocks is called.
class hkFreeList
{
	public:

		HK_DECLARE_PLACEMENT_ALLOCATOR();

		typedef void (HK_CALL *MemoryWalkCallback)(void* start, hk_size_t size,hkBool allocated,int pool,void* param);

			/// The size of an element, and the alignment all elements must have
		hkFreeList( hk_size_t elementSize, hk_size_t alignment, hk_size_t blockSize, hkMemoryAllocator* elementAllocator = HK_NULL, hkMemoryAllocator* blockAllocator = HK_NULL);

			/// Default Ctor - doesn't set up any allocation. Can be subsequently configured with a call to init'.
		hkFreeList();

			/// Dtor
		~hkFreeList() { freeAllMemory(); }

		struct Element
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,Element);
			Element* m_next;
		};

			// The block is 16 byte aligned, as are allocations from the large block allocator. Which is nice - and a
			// requirement for the alignment.
		struct Block
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,Block);
			Block* m_next;					///< The next block in the list, or NULL if there isn't one
			hkUint8* m_elementsAlloc;		///< The address of the allocation returned by the allocator. The size is the 'blockSize'
			hkUint8* m_elements;			///< The start of the elements payload
			hk_size_t m_numElements;		///< Number of elements in this block (needed as the alignment may mean not same as the freelist numBlockElements)
		};

			/// Initialize, can be called multiple time to reconfigure how a freelist is used.
			/// Will free all elements that are allocated on the freelist.
		void init(hk_size_t elementSize, hk_size_t alignment, hk_size_t blockSize, hkMemoryAllocator* elementAllocator = HK_NULL, hkMemoryAllocator* blockAllocator = HK_NULL);

			/// Returns true if alloc will succeed
		HK_FORCE_INLINE hkBool canAlloc();

			/// Allocates a chunk of memory
		HK_FORCE_INLINE void* alloc();

			/// Returns the chunk of memory to the pool. The chunk _must_ have been allocated from this pool
		HK_FORCE_INLINE void free(void* data);

			/// Allocate a batch
		HK_FORCE_INLINE void allocBatch(void** out,int sizeIn);

			/// Free a batch
		HK_FORCE_INLINE void freeBatch(void** in,int sizeIn);

			/// Frees all of the memory used by the memory pool
		void freeAllMemory();

			/// Makes all of the memory allocated available
		void freeAll();

			/// Get the element size
		hk_size_t getElementSize() const { return m_elementSize; }

			/// Does checks to see if this memory is ok
		hkBool isOk() { return _checkFreeBlocks(); }

			/// Returns true if there is an element available (without extra allocation)
		hkBool isFreeElementAvailable() const { return m_free || m_top < m_blockEnd; }

			/// Simple memory stats
		void getMemoryStatistics(hkMemoryAllocator::MemoryStatistics& stats);

			/// Finds blocks which are not used, and puts them on the 'freeBlocks' list
			/// Returns number of blocks found, or <0 if couldn't do the work
		int findGarbage();

			/// Returns the number of blocks examined.
			/// The free list holds state, so will only examine blocks after ones previously examined.
			/// Returns true when hits the end of all the blocks, will start on the first block when called again
		hkBool incrementalFindGarbage(int numBlocks, int& numBlocksOut);

			/// Collect
		inline void garbageCollect();

			/// Frees all of the free blocks
		int freeAllFreeBlocks();

			/// Returns true if there are freeblocks in the list
		hkBool hasFreeBlocks() const { return m_freeBlocks != HK_NULL; }

			/// Get the total amount of free elements that are available
		hk_size_t getNumFreeElements() const { return m_numFreeElements; }

			/// Get the total member of elements, free and used that have been allocated
		hk_size_t getTotalNumElements() const { return m_totalNumElements; }

			/// Get the total number of elements that have been used
		hk_size_t getNumUsedElements() const { return m_totalNumElements - m_numFreeElements; }

			/// Get the current allocation block size
		hk_size_t getBlockSize() const { return m_blockSize; }

		int _getTotalNumBlocks(Block* blocks) const
		{
			int num = 0;
			while (blocks)
			{
				num++;
				blocks = blocks->m_next;
			}
			return num;
		}

			/// Will callback for each free and allocated memory block
		void walkMemory(MemoryWalkCallback callback,int pool,void* param);

			/// Blocksize for elements
		static hk_size_t bestBlockSize(hk_size_t elementSpace, hk_size_t align);

			// Internal method to get the block lists used for mem walking.
		int _getSortedBlockHeads( const Block** heads, int numHeads );

	protected:

			// Make more space and return a new block from the space that was allocated. If not possible to allocate
			// return HK_NULL
		void* addSpace();

			// Returns the amount free elements
		hk_size_t _calcNumFreeElements() const;

			// Returns the total amount elements, free or used
		hk_size_t _calcTotalNumElements() const;

			// Returns the number of used elements
		hk_size_t _calcNumUsedElements() const { return _calcTotalNumElements() - _calcNumFreeElements(); }

			// Goes down the linked list working out how many elements there are
		static hk_size_t _calcTotalNumElements(Block* cur);

			// Frees a link list of blocks
		int _freeBlocks(Block* cur);

			// Calculates the sum size of a linked list of blocks
		hk_size_t _calculateBlockAllocatedSize(Block* cur);
	
			// Sets up allocation - assumes all memory has been freed
		void _init(hk_size_t elementSize, hk_size_t align, hk_size_t blockSize, hkMemoryAllocator* elementAllocator, hkMemoryAllocator* blockAllocator);

		static HK_FORCE_INLINE hkBool _compareBlocks( const Block* a, const Block* b ) { return a < b; }
		static HK_FORCE_INLINE hkBool _compareElements( const Element* a, const Element* b ) { return a < b; }

			/// Add all the elements of block to the free list
		void _addBlockElements(Block* block)
		{
			m_top = block->m_elements;
			m_blockEnd = block->m_elements + block->m_numElements * m_elementSize;
		}

		void _walkMemoryBlockList(Block* block, MemoryWalkCallback callback, int pool, void* param);

		hkBool _checkFreeBlocks();

			/// Analyses the block to see if all of its elements are free, if so it moves the block to the m_freeBlocks list, and returns true
			/// Also in that case removes all the elements from the m_free list
			/// If they aren't all free, it will reorder the free blocks so they will be returned contiguously from memory
		hkBool32 _calcBlockFree(Block* block);

			/// Move any elements which are in the "unpartitioned" space of the current block, to the free list (m_free).
		void _moveTopToFree();

	protected:

			/// Singly linked list of free elements. NULL terminated
		Element* m_free;

			/// The size of a single element
		hk_size_t m_elementSize;

			/// The last block incrementally garbage collected. HK_NULL if at start of list.
			/// Should only point to used blocks (i.e., blocks in the m_activeBlocks list)
		Block* m_lastIncrementalBlock;

			// The active blocks
		Block* m_activeBlocks;

			/// The free blocks (blocks which haven't seen use in current allocation)
		Block* m_freeBlocks;

		hk_size_t m_blockSize;
		hk_size_t m_align;
		hk_size_t m_numBlockElements;			///< The number of elements in an aligned block
		//hk_size_t m_maxBlockSize;

			// The top of the current block, above it memory is free up until blockEnd is hit
		hkUint8* m_top;
		hkUint8* m_blockEnd;

		hkMemoryAllocator* m_elementAllocator;			///< Underlying element allocator
		hkMemoryAllocator* m_blockAllocator;			///< Underlying block allocator

			// A cached value of the total amount of elements
		hk_size_t m_totalNumElements;

	public:
			// A cache of the total amount of free elements
		hk_size_t m_numFreeElements;
};

#include <Common/Base/Memory/Allocator/FreeList/hkFreeList.inl>

#endif // HK_FREELIST

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
