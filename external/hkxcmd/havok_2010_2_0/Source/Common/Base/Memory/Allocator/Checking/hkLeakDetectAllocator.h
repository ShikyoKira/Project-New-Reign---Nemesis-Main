/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_hkLeakDetectAllocator_H
#define HKBASE_hkLeakDetectAllocator_H

#include <Common/Base/System/StackTracer/hkStackTracer.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

class hkDebugMemorySnapshot;

	/// Allocator for detecting memory leaks.
	/// This class is useful for detecting leaks in a single allocator.
	/// Mostly you'll want to use the hkCheckingMemorySystem which hooks
	/// all allocators in all threads.
class hkLeakDetectAllocator : public hkMemoryAllocator
{
	//+hk.MemoryTracker(ignore=True)
	public:
		typedef void (HK_CALL *OutputStringFunc)(const char* s, void* userData);

			/// initialize.
			/// The \a debugAllocator will be used to allocate internal debug tracking information
		void init(hkMemoryAllocator* childAllocator, hkMemoryAllocator* debugAllocator, OutputStringFunc output, void* outputUserData);

		void quit();

		virtual void* blockAlloc( int numBytes );
		virtual void blockFree( void* p, int numBytes );
		virtual void* bufAlloc( int& reqNumInOut );
		virtual void bufFree( void* p, int num );
		virtual void* bufRealloc( void* pold, int oldNum, int& reqNumInOut );

		hkResult getMemorySnapshot(hkDebugMemorySnapshot& snapshot);

		struct AllocInfo
		{
			int traceId;
			int size;
			hkBool isBuf;
		};

		virtual void getMemoryStatistics( MemoryStatistics& out );
		virtual int getAllocatedSize(const void* obj, int nbytes);

	protected:

		void* checkedAlloc( hkBool32 isBuf, int numBytes );
		void checkedFree( hkBool32 isBuf, void* p, int numBytes );
		void danger(const char* message, const void* ptr, const AllocInfo& info) const;

		hkStackTracer* m_stackTracer;
		hkStackTracer::CallTree m_callTree;
		mutable hkCriticalSection m_criticalSection;

		hkMemoryAllocator* m_childAllocator;
		hkMemoryAllocator* m_debugAllocator;

		OutputStringFunc m_outputFunc;
		void* m_outputFuncArg;

		typedef hkMapBase<hkUlong,AllocInfo> MapType;

		struct Dynamic
		{
			// remove this struct when hkMap doesn't alloc in it ctor
			HK_DECLARE_PLACEMENT_ALLOCATOR();
			Dynamic(hkMemoryAllocator* a) {}
			void quit(hkMemoryAllocator* a)
			{
				m_activePointers.clearAndDeallocate(*a);
			}

			MapType m_activePointers;
		};
		Dynamic* m_dynamic;

		hk_size_t m_inUse;
};

#endif //HKBASE_hkLeakDetectAllocator_H

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
