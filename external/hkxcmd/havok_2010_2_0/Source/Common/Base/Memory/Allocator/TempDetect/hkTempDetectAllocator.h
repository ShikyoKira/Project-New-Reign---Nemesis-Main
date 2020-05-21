/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_hkTempDetectAllocator_H
#define HKBASE_hkTempDetectAllocator_H

#include <Common/Base/System/StackTracer/hkStackTracer.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

	/// Detects cases when the temp allocator should be used instead of the heap.
class hkTempDetectAllocator : public hkMemoryAllocator
{
	public:
		HK_DECLARE_PLACEMENT_ALLOCATOR();

		hkTempDetectAllocator();

		typedef void (HK_CALL *OutputStringFunc)(const char* s, void* userData);
		void init(hkMemoryAllocator* a, OutputStringFunc output, void* outputUserData);
		void quit();

		void advanceFrame();

		virtual void* blockAlloc( int size );
		virtual void blockFree( void* p, int size );

		virtual void* bufAlloc( int& reqNumInOut );
		virtual void bufFree( void* p, int numElem );
		virtual void* bufRealloc( void* pold, int oldNum, int& reqNumInOut );

		virtual void getMemoryStatistics( MemoryStatistics& u );
		virtual int getAllocatedSize(const void* obj, int nbytes);

	protected:

		void internalFree(void* p, int size);
		void* internalAlloc(void* p, int size);

	public:

		typedef hkStackTracer::CallTree::TraceId TraceId;
		struct AllocInfo
		{
			hkUint64 threadId;
			TraceId allocId;
			int size;
		};

		hkCriticalSection m_lock;
		hkMemoryAllocator* m_child;
		hkStackTracer::CallTree m_callTree;
		hkStackTracer* m_tracer;

			// Map of allocated address to callstack
		hkMapBase<void*, AllocInfo> m_allocs;
			// For freed memory map alloc locations to freed locations.
			// Special case: the free id is zero if we've given up on a pair.
			// This can happen when there are multiple freeing locations for a given allocation
			// or if the allocating thread is different to the freeing thread.
		hkMapBase<TraceId, TraceId> m_freeFromAlloc;

		struct SizeInfo
		{
			int minSize;
			int maxSize;
			hkUlong total;
			int count;
		};
		hkMapBase<TraceId, SizeInfo> m_sizeFromAlloc;

		OutputStringFunc m_outputFunc;
		void* m_outputFuncArg;
};

#endif // HKBASE_hkTempDetectAllocator_H

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
