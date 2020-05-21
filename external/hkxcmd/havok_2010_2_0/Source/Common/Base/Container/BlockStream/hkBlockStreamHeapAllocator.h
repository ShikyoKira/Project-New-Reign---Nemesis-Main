/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BLOCK_STREAM_HEAP_ALLOCATOR_H
#define HK_BLOCK_STREAM_HEAP_ALLOCATOR_H

#include <Common/Base/Container/BlockStream/hkBlockStreamBlock.h>

template<int BLOCKSIZE>
class hkBlockStreamHeapAllocator
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkBlockStreamHeapAllocator );

			/// Allow the BlockStream template to obtain the block size from this allocator.
		enum 
		{
			BLOCK_SIZE = BLOCKSIZE	
		};

			/// Not needed for this allocator, but expected by the hkBlockStream template.
		typedef void UnderlyingAllocator;

		typedef hkBlockStreamBlock<BLOCK_SIZE> Block;

			/// Allocate a block
		HK_FORCE_INLINE Block* blockAlloc() 
		{ 
			Block* b = hkAllocateChunk<Block>(1, HK_MEMORY_CLASS_BASE );
			HK_ON_DEBUG( b->m_allocator = HK_NULL );
			return b;
		}

			/// Deallocate a block
		HK_FORCE_INLINE void blockFree( Block* block ) 
		{ 
			hkDeallocateChunk<Block>( block, 1, HK_MEMORY_CLASS_BASE );
		}

		HK_FORCE_INLINE void blockFreeBatch( Block** blocks, int nblocks ) 
		{ 
			for ( int i = 0; i < nblocks; ++i ) 
			{ 
				blockFree( blocks[i] );
			}
		}

			/// Not needed for this allocator, but expected by the hkBlockStream template.
		HK_FORCE_INLINE UnderlyingAllocator* getUnderlyingAllocator() const
		{
			return HK_NULL;
		}
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
