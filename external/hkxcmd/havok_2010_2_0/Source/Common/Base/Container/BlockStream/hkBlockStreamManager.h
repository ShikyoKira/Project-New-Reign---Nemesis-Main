/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BLOCK_STREAM_MANAGER_H
#define HK_BLOCK_STREAM_MANAGER_H

#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>
#include <Common/Base/Container/BlockStream/hkBlockStreamBlock.h>
#include <Common/Base/Container/BlockStream/hkBlockStream.h>

#define HK_BLOCK_ALIGNMENT 128

class hkBlockStreamTlAllocator;
template<typename Allocator> class hkBlockStreamBase;

class hkBlockStreamAllocator
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkBlockStreamAllocator );

		enum {
			BLOCK_SIZE = 1024 * 4
		};

		typedef hkBlockStreamBlock<BLOCK_SIZE> Block;
			
		void init( int byteSize );

		void init( void* buffer, int byteSize );

		void clear();

		hkBlockStreamAllocator();

		~hkBlockStreamAllocator();

		void addMemory( void* buffer, int byteSize );

		Block* blockAlloc();

		void blockFree(Block* block);

		void blockAllocBatch( Block** blocksOut, int nblocks );

		void blockFreeBatch( Block** blocks, int nblocks );

		/// Get the current bytes used. Note that data in the thread local allocator will
		/// count as bytes used (as they are not available to other threads in a worst case
		/// scenario.
		int getBytesUsed();

		/// The the peak usage
		int getMaxBytesUsed();

	private:

		void setFreeList();

	public:

		mutable hkCriticalSection m_criticalSection;

		Block* m_storage;

		hkArray< Block* > m_freeList;

		int		m_capacity;
		int     m_minFreeListSize;	// for peak memory usage
		hkBool	m_dontDeallocate;
};


/// Thread local allocator, which caches a number of blocks
class hkBlockStreamTlAllocator
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkBlockStreamTlAllocator );

			/// Allow the BlockStream template to obtain the block size from this allocator.
		enum
		{
			BLOCK_SIZE = hkBlockStreamAllocator::BLOCK_SIZE
		};

		typedef hkBlockStreamAllocator UnderlyingAllocator;

		hkBlockStreamTlAllocator(hkBlockStreamAllocator* bsAllocator): m_blockStreamAllocator( bsAllocator ), m_numFreeBlocks(0){ }

		~hkBlockStreamTlAllocator(){ clear(); }

		/// Allocate a block
		hkBlockStreamAllocator::Block* blockAlloc();

		/// Deallocate a block
		void blockFree(hkBlockStreamAllocator::Block* block);

		/// Free a batch
		void blockFreeBatch( hkBlockStreamAllocator::Block** blocks, int nblocks );

		/// Clear all locally cached blocks
		void clear(); 

		HK_FORCE_INLINE UnderlyingAllocator* getUnderlyingAllocator() const
		{
			return m_blockStreamAllocator;
		}

		enum
		{
			BATCH_ALLOC_SIZE		= 8,	///< The number of blocks freed/allocated in one go in the m_blockStreamManager
			MAX_FREE_LIST_SIZE		= 16,	///< The maximum number of elements cached locally
		};

	protected:
		void checkConsistency( );

	public:

		hkPadSpu<hkBlockStreamAllocator*> m_blockStreamAllocator;

		hkPadSpu<int>	m_numFreeBlocks;
		hkBlockStreamAllocator::Block* m_freeBlocks[MAX_FREE_LIST_SIZE];
};

#endif

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
