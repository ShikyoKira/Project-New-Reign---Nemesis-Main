/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_hkThreadMemory_H
#define HKBASE_hkThreadMemory_H

	/// This class is a block cache used in conjunction with another allocator.
	///
	/// Notes:
	///    - It is highly optimized for handling memory for a single thread. It
	///      is doing so by caching a limited number of blocks in a thread local
	///      free list. There is a several free lists for different sizes of blocks.
	///    - Each free list (for each size) can hold only a maximum number of items (m_maxNumElemsOnFreeList)
	///    - If maxNumElemsOnFreeList==0 than the free list is not checked at all and the hkThreadMemory
	///      calls the appropriate parent allocator functions
	///    - If for a given size, there is no free block on the free list, some blocks are fetched by calling
	///      parents blockAllocBatch() method
	///    - If a block is freed, it is assumed that this block was allocated by this memory manager, its
	///      corresponding free list is found (by rounding up the size of the block to the next suitable
	///      block size) and the block added to the free list.
	///      If the number of elements on this particular free list is exceeding m_maxNumElemsOnFreeList, some
	///      blocks are forwarded to the parents blockFreeBatch() method.
	///
	/// Notes:
	///     - all allocations should be 16 byte aligned.
	///     - Only if you use this class with the pool memory, you are allowed to set maxNumElemsOnFreeList != 0
class hkThreadMemory : public hkMemoryAllocator
{
	//+hk.MemoryTracker(ignore=True)
	public:
		HK_DECLARE_PLACEMENT_ALLOCATOR();

		struct FreeList
		{
			//+hk.MemoryTracker(ignore=True)
			FreeList() : m_head(HK_NULL), m_numElem(0) {}
			void put( void* p )
			{
				m_numElem += 1;
				FreeElem* fl = static_cast<FreeElem*>(p);
				fl->m_next = m_head;
				m_head = fl;
			}
			void* get()
			{
				if(void* p = m_head)
				{
					m_numElem -= 1;
					m_head = m_head->m_next;
					return p;
				}
				return HK_NULL;
			}

			struct FreeElem	{ FreeElem* m_next; };
			FreeElem* m_head;
			int m_numElem;
		};

		virtual void* blockAlloc( int numBytes );
		virtual void blockFree( void* p, int numBytes );
		virtual void* bufAlloc( int& reqNumInOut );
		virtual void bufFree( void* p, int numElem );
		virtual void* bufRealloc( void* p, int oldNum, int& reqNumInOut  );
		virtual void blockAllocBatch(void** ptrsOut, int numPtrs, int blockSize);
		virtual void blockFreeBatch(void** ptrsIn, int numPtrs, int blockSize);

		hkThreadMemory();
			/// Creates a new instance of a thread memory.
		hkThreadMemory( hkMemoryAllocator* mainMemoryManager );

			///
		void setMemory( hkMemoryAllocator* mainMemoryManager, int maxNumElemsOnFreeList=8 );

			/// Gives all local memory back to the main memory manager
		virtual void releaseCachedMemory();

			/// Calls releaseCachedMemory
		HK_FORCE_INLINE virtual ~hkThreadMemory(){ releaseCachedMemory(); }

			/// Get a simply memory statistics of cached blocks
		hk_size_t getCachedSizeUnchecked();

		virtual void getMemoryStatistics( MemoryStatistics& u );
		virtual int getAllocatedSize(const void* obj, int nbytes);

	public:

		enum
		{
			
			
			
				// We are aligning on a 64 byte boundary,
				// as all the data layout of all major functions is optimized for
				// the 64 bytes cache line width of playstation
			PAGE_ALIGN  = 64,

				// All small rows must align with this shift
			MEMORY_SMALL_BLOCK_RSHIFT_BITS = 4,		
				/// Added before the shift
			MEMORY_SMALL_BLOCK_ADD = 0xf,

				// The number of small chunk sizes
			MEMORY_MAX_SMALL_ROW  = 13,

				// The number of small and large chunk sizes
			MEMORY_MAX_ALL_ROW = (MEMORY_MAX_SMALL_ROW+4),

				// The largest small block we allocate from this pool - this is specially sized to match sizeof(hkpRigidBody)
			MEMORY_MAX_SIZE_SMALL_BLOCK  = 640, // (512 + 32) aligned to 128 alignment

				// The largest large block we allocate from this pool
			MEMORY_MAX_SIZE_LARGE_BLOCK = 8192,

				// The low bits we ignore when indexing into the large arrays
			MEMORY_LARGE_BLOCK_RSHIFT_BITS = 10,

				// How much we allocate when the small pool becomes full
			MEMORY_SYSTEM_PAGE_SIZE = 8192,

				// Debugging
			MEMORY_MAGIC_NUMBER = 0x3425234
		};

			/// Get the row of a given size. The size has to be less than the largest small block size
		HK_FORCE_INLINE int getRow(int nbytes) const;

			/// Get the row of a given size. The size has to be less than the largest small block size
			/// This is faster if the size is known at compile time
		static HK_FORCE_INLINE int HK_CALL constSizeToRow( int size );

			/// returns the memory block size of the elements per row
		HK_FORCE_INLINE int rowToSize( int row ) const;

	protected:

			// thread memory talks to main memory in batches of this size
		enum { BATCH_SIZE=4 };

			// Called when an allocation from an empty row is requested.
		void* onRowEmpty(int row);
			// Called when an deallocation from a full row is requested. p is a ptr that will be added to that row
		void onRowFull(int row, void* p);
			// Clear all of the pointers on that row. 
		void clearRow(int rowIndex);
			//
		void initLookupTables();

		//
		//	Internal public section
		//
	public:

			// pointer to the main memory
		hkMemoryAllocator* m_memory;

			// the maximum number of elements on each free list before the parent hkMemory is called
		int m_maxNumElemsOnFreeList;

			// free list for blocks of each size
		FreeList m_free_list[MEMORY_MAX_ALL_ROW];

			// a lookup table of size of each block size
		int m_row_to_size_lut[MEMORY_MAX_ALL_ROW];

			// a lookup table of sizes to small block size
		char m_small_size_to_row_lut[(MEMORY_MAX_SIZE_SMALL_BLOCK >> MEMORY_SMALL_BLOCK_RSHIFT_BITS)+1];

			// a lookup table of sizes to large block size
		int m_large_size_to_row_lut[ (MEMORY_MAX_SIZE_LARGE_BLOCK >> MEMORY_LARGE_BLOCK_RSHIFT_BITS) ];
};


#include <Common/Base/Memory/Allocator/Thread/hkThreadMemory.inl>

#endif // HKBASE_hkThreadMemory_H

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
