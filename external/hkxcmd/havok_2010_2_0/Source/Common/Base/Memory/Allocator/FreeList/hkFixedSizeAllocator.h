/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIXED_SIZE_ALLOCATOR_H
#define HK_FIXED_SIZE_ALLOCATOR_H

#include <Common/Base/Memory/Allocator/hkMemoryAllocator.h>
#include <Common/Base/Memory/Allocator/FreeList/hkFreeList.h>

class hkFixedSizeAllocator: public hkMemoryAllocator
{
	public:
		HK_DECLARE_PLACEMENT_ALLOCATOR();

		virtual void* blockAlloc( int numBytes );
		virtual void blockFree( void* p, int numBytes );
		virtual void* bufAlloc( int& reqNumBytesInOut );
		virtual void bufFree( void* p, int numBytes );
		virtual void* bufRealloc( void* pold, int oldNumBytes, int& reqNumBytesInOut );
		virtual void blockAllocBatch(void** ptrsOut, int numPtrs, int blockSize);
		virtual void blockFreeBatch(void** ptrsIn, int numPtrs, int blockSize);
		virtual void getMemoryStatistics( MemoryStatistics& u );
		virtual int getAllocatedSize(const void* obj, int nbytes);

			/// Ctor.
		void init(hk_size_t blockSize, hk_size_t align, hk_size_t allocatorBlockSize, hkMemoryAllocator* allocator);

	
		hkFreeList m_freeList;
};

#endif // HK_FIXED_SIZE_ALLOCATOR_H

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
