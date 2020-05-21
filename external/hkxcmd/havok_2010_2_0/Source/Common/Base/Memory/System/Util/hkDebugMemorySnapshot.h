/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_DEBUG_MEMORY_SNAPSHOT_H
#define HK_DEBUG_MEMORY_SNAPSHOT_H

#include <Common/Base/System/StackTracer/hkStackTracer.h>


struct hkMemoryPointerInfo
{
	int		m_numBytes;
	hkUlong m_stackTraces[16];
	int		m_numStackTrace;
};

class hkDebugMemorySnapshot
{
    public:
    	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkDebugMemorySnapshot);

		typedef hkMemoryPointerInfo PointerInfo;

		hkDebugMemorySnapshot():m_pointers(HK_NULL),m_info(HK_NULL),m_numPointers(0) {}

		~hkDebugMemorySnapshot() { reset(); }
			/// Empties out all allocations
		void reset();

			/// Subtract all of the allocations in rhs
		void subtract(hkDebugMemorySnapshot& rhs);

			/// Sorts the pointers into ascending order
		void ascendingOrder();


		void** m_pointers;
		PointerInfo* m_info;
		int m_numPointers;

	protected:

		static HK_FORCE_INLINE hkBool _comparePointers( void** a, void** b )
		{
			return (char*)(*a) < (char*)(*b);
		}
};

#endif // HK_DEBUG_MEMORY_SNAPSHOT_H

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
