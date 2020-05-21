/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MALLOC_ALLOCATOR_H
#define HK_MALLOC_ALLOCATOR_H

#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

	/// Simple manager which forwards all calls to malloc and free.
class hkMallocAllocator : public hkMemoryAllocator
{
	//+hk.MemoryTracker(ignore=True)
	public:
		hkMallocAllocator(int align=16) : m_align(align), m_currentUsed(0), m_peakUse(0)  { }

		virtual void* blockAlloc( int numBytes );
		virtual void blockFree( void* p, int numBytes );

		virtual void getMemoryStatistics( hkMemoryAllocator::MemoryStatistics& u );
		virtual int getAllocatedSize(const void* obj, int nbytes);
		virtual void resetPeakMemoryStatistics();

		int m_align;

		static hkMemoryAllocator *m_defaultMallocAllocator;

	protected:

		hkUint32 m_currentUsed;
		hkUint32 m_peakUse;
};

#endif // HK_MALLOC_ALLOCATOR_H

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
