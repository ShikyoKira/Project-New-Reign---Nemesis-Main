/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_MONITOR_ALLOCATOR_H
#define HKBASE_MONITOR_ALLOCATOR_H

	/// Records the memory request to the monitor stream and forwards to a child allocator.
	/// Use this with the details statistics to see the memory requests interleaved with the
	/// monitor timers. Note that this will require much more memory than the monitor timers.
class hkMonitorAllocator : public hkMemoryAllocator
{
	public:
		HK_DECLARE_PLACEMENT_ALLOCATOR();

		hkMonitorAllocator() : m_child(HK_NULL), m_monitorTag(HK_NULL) {}
		hkMonitorAllocator(hkMemoryAllocator* child, const char* mtag);

		virtual void* blockAlloc( int size );
		virtual void blockFree( void* p, int size );

		hkMemoryAllocator* m_child;
		const char* m_monitorTag;
};

#endif // HKBASE_MONITOR_ALLOCATOR_H

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
