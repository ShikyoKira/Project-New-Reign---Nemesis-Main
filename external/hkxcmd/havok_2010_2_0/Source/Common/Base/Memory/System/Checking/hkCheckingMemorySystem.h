/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_hkCheckingMemorySystem_H
#define HKBASE_hkCheckingMemorySystem_H

#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/System/StackTracer/hkStackTracer.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/Memory/Allocator/Checking/hkPaddedAllocator.h>
#include <Common/Base/Memory/Allocator/Checking/hkDelayedFreeAllocator.h>
#include <Common/Base/Memory/Allocator/Stats/hkStatsAllocator.h>

class hkDebugMemorySnapshot;

	/// Debugging memory system checks for some common errors.
	/// It detects memory leaks and some bad memory writes. The default is
	/// to perform a stack trace (if the platform supports it) on every allocation
	/// to pinpoint leaks. This may be disabled for increased speed.
	///
	/// Several other logical errors are detected, such as allocating and freeing
	/// memory with incompatible allocators.
	///
	/// This class owns several proxy allocators which forward requests
	/// back to central methods along with a context indicating where the
	/// request came from.
class hkCheckingMemorySystem : public hkMemorySystem
{
	//+hk.MemoryTracker(ignore=True)
	public:

		HK_DECLARE_PLACEMENT_ALLOCATOR();

			/// Checks the allocator can perform.
		enum CheckBits
		{
				/// No checks, just forward the request.
			CHECK_NONE = 0,
			// CHECK_UNUSED= 1, available for reuse
				/// Check for matching free for each alloc.
			CHECK_LEAKS = 2,
				/// Store a callstack for each allocation, especially useful for leak checking.
			CHECK_CALLSTACK = 4,
				/// Pad each allocation with some canary bytes.
			CHECK_PAD_BLOCK = 8,
				/// Cache some free blocks to detect use-after-free errors.
			CHECK_DELAYED_FREE = 16,

				/// Default is everything enabled.
			CHECK_DEFAULT = CHECK_LEAKS|CHECK_PAD_BLOCK|CHECK_DELAYED_FREE|CHECK_CALLSTACK
		};

			///
		enum AllocatorBits
		{
				/// Freeing thread must also be the allocating thread.
			ALLOCATOR_INVALID = 0,
			ALLOCATOR_HEAP = 1,
			ALLOCATOR_TEMP = 2,
			ALLOCATOR_SOLVER = 4,
			ALLOCATOR_MAX
		};
		typedef hkFlags<AllocatorBits,int> AllocatorFlags;


			///
		struct AllocationContext
		{
			AllocationContext() : threadId(0), curInUse(0), peakInUse(0) {}
			//+hk.MemoryTracker(ignore=True)
				/// Thread identifier
			hkUint64 threadId;
				///
			AllocatorFlags flags;
				/// Stats
			mutable int curInUse;
				/// Stats
			mutable int peakInUse;
		};

			/// This allocator forwards requests to a checking memory allocator along with a context.
		struct AllocatorForwarder : public hkMemoryAllocator
		{
			//+hk.MemoryTracker(ignore=True)
			HK_DECLARE_PLACEMENT_ALLOCATOR();

			AllocatorForwarder() : m_parent(HK_NULL) {}
			virtual void* blockAlloc( int numBytes );
			virtual void blockFree( void* p, int numBytes );
			virtual void* bufAlloc( int& reqNumInOut );
			virtual void bufFree( void* p, int num );
			virtual void* bufRealloc( void* pold, int oldNum, int& reqNumInOut );
			virtual void getMemoryStatistics( MemoryStatistics& u );
			virtual int getAllocatedSize(const void* obj, int nbytes);
		
			virtual void resetPeakMemoryStatistics();

			hkCheckingMemorySystem* m_parent;
			AllocationContext m_context;
		};

			/// Callback printing function type
		typedef void (HK_CALL *OutputStringFunc)(const char* s, void* userData);

		hkCheckingMemorySystem();

			///
		void init(hkMemoryAllocator* a, OutputStringFunc output, void* outputUserData, CheckBits checks=CHECK_DEFAULT);

			///
		hkBool isInit(); 

			///
		hkResult quit();

		void* checkedAlloc( hkBool32 isBuf, const AllocationContext& context, int numBytes );
		void checkedFree( hkBool32 isBuf, const AllocationContext& context, void* p, int numBytes );

		virtual hkResult getMemorySnapshot(hkDebugMemorySnapshot& snapshot);
		virtual hkResult getAllocationCallStack(void* ptr, hkUlong* callStack, int& stackSize, hk_size_t& allocSize);


		virtual void debugTagAddress(const void* baseAddress, const void* tag);

			/// Perform an internal consistency check.
		hkBool32 isOk() const;

		struct AllocInfo
		{
			//+hk.MemoryTracker(ignore=True)
			hkUint64 m_threadId;
			hkUint64 m_allocOrder;
			const void* m_tagData;
			AllocatorFlags m_flags;
			hkStackTracer::CallTree::TraceId m_traceId;
			int m_size;
			int m_locks;
		};

		const void* debugFindBaseAddress(const void* p, int nbytes);
		void debugLockBlock(const void* p);
		void debugUnlockBlock(const void* p);

		//
		// Memory system methods.
		//
		virtual hkMemoryRouter* mainInit(const FrameInfo& info, Flags f=FLAG_ALL);
		virtual hkResult mainQuit(Flags f=FLAG_ALL);
		virtual void threadInit(hkMemoryRouter& r, const char* name, Flags f=FLAG_ALL);
		virtual void threadQuit(hkMemoryRouter& r, Flags f=FLAG_ALL);
		virtual void printStatistics(hkOstream& ostr);
		virtual void getHeapStatistics(hkMemoryAllocator::MemoryStatistics& stats);
		virtual void advanceFrame();
		virtual void garbageCollectShared();
		virtual hkResult walkMemory(MemoryWalkCallback callback, void* param);
		virtual hkMemoryAllocator* getUncachedLockedHeapAllocator();

	protected:

			/// Create a new forwarding allocator
		AllocatorForwarder* newAllocator(AllocatorFlags flags, hkUint64 tid);
			/// Destroy all allocators for a given thread.
		void deleteAllocators( hkUint64 tid );
			///
		void deleteAllocator( hkMemoryAllocator* a );

		void defaultLeakReport();
		void summaryLeakReport();

		void dumpLeak(const hkArrayBase<int>& owned, const hkArrayBase<hkUlong>& addrs, const hkArrayBase<AllocInfo>& allocs);

	protected:

		typedef hkMapBase<hkUlong,AllocInfo> MapType;

		hkStackTracer* m_stackTracer;
		hkStackTracer::CallTree m_callTree;
		mutable hkCriticalSection m_section;

			/// The allocator we use - usually either the raw allocator or the bounds checking one.
		hkMemoryAllocator* m_baseAllocator;
		hkPaddedAllocator m_paddedAllocator; // checks for under/overruns
		hkDelayedFreeAllocator m_delayedFreeAllocator; // 
		hkMemoryAllocator* m_rawAllocator; // the one given to init
		hkStatsAllocator m_debugAllocator;
		hkStatsAllocator m_callTreeAllocator;

			/// These forward tagged requests to the checkedAlloc and checkedFree
		hkArrayBase<AllocatorForwarder*> m_allocators;

		hkFlags<CheckBits, int> m_checkFlags;

		MapType m_activePointers;
		hk_size_t m_currentInUse;
		hk_size_t m_peakInUse;
		
		hkUint64 m_allocOrder;

		OutputStringFunc m_outputFunc;
		void* m_outputFuncArg;

		FrameInfo m_frameInfo;

		hk_size_t m_sumAllocatedStackMemory;	 // sum of all stack memories
		hkMemoryRouter m_mainRouter;

		void danger(const char* message, const void*, const AllocInfo& info) const;
};

#endif // HKBASE_hkCheckingMemorySystem_H

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
