/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_hkOptimizerMemorySystem_H
#define HKBASE_hkOptimizerMemorySystem_H

#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Solver/hkSolverAllocator.h>
#include <Common/Base/Memory/Allocator/TempDetect/hkTempDetectAllocator.h>
#include <Common/Base/Memory/Allocator/Lifo/hkLifoAllocator.h>

	/// Watches memory allocations and suggests ways to improve allocation patterns.
	/// This is slower than the default memory system. It is intended for use in debug mode
	/// to detect situations which could use better memory allocation patterns.
	/// Currently the only check implemented is to suggest heap allocations which should
	/// be moved to temp allocations.
class hkOptimizerMemorySystem : public hkMemorySystem
{
	//+hk.MemoryTracker(ignore=True)
public:
		enum { THREAD_MAX = 32 };
		typedef void (HK_CALL *OutputStringFunc)(const char* s, void* userData);

		enum DetectBits
		{
			DETECT_NONE = 0,
			DETECT_TEMP = 1,
			DETECT_LIFO = 2,

			DETECT_ALL = DETECT_TEMP | DETECT_LIFO
		};
		typedef hkFlags<DetectBits, hkUint32> DetectFlags;

		struct LifoCheckAllocator : public hkMemoryAllocator
		{
			virtual void* blockAlloc( int numBytes );
			virtual void blockFree( void* p, int numBytes );
			virtual void* bufRealloc( void* pold, int oldNumBytes, int& reqNumBytesInOut );
			virtual void getMemoryStatistics( MemoryStatistics& u );
			virtual int getAllocatedSize(const void* obj, int nbytes);

			void init(hkMemoryAllocator* base, OutputStringFunc func, void* funcArg);
			void quit();
			hkArrayBase<void*> m_allocs;
			hkMemoryAllocator* m_allocator;
			OutputStringFunc m_outputFunc;
			void* m_outputFuncArg;
			hkStackTracer* m_tracer; // borrowed ref from m_tempDetect.m_tracer
		};

		struct ThreadData
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,ThreadData);

			ThreadData() : m_inUse(false) { }

			LifoCheckAllocator m_lifoChecker;
			hkBool m_inUse;
			hk_size_t m_stackSize;
		};

		hkOptimizerMemorySystem();

		virtual hkMemoryRouter* mainInit(const FrameInfo& info, Flags f=FLAG_ALL);
		virtual hkResult mainQuit(Flags f=FLAG_ALL);
		virtual void threadInit(hkMemoryRouter& r, const char* name, Flags f=FLAG_ALL);
		virtual void threadQuit(hkMemoryRouter& r, Flags f=FLAG_ALL);

		virtual void printStatistics(hkOstream& ostr);
		virtual void getHeapStatistics(hkMemoryAllocator::MemoryStatistics& stats);

		virtual void advanceFrame() { m_tempDetector.advanceFrame(); }
		virtual hkResult walkMemory(MemoryWalkCallback callback, void* param) { return HK_FAILURE; }

		void init(hkMemoryAllocator* a, OutputStringFunc output, void* outputUserData, DetectFlags detect);
		hkMemoryAllocator* getAllocator() { return m_baseAllocator; }
		void setAllocator(hkMemoryAllocator* a) { HK_ASSERT(0x62b32838, m_baseAllocator==HK_NULL); m_baseAllocator = a; }
		virtual hkMemoryAllocator* getUncachedLockedHeapAllocator();

	protected:

		FrameInfo m_frameInfo;
		hkMemoryAllocator* m_baseAllocator;
		hkMemoryRouter m_mainRouter;
		hkSolverAllocator m_solverAllocator;
		hkTempDetectAllocator m_tempDetector;
		OutputStringFunc m_outputFunc;
		void* m_outputFuncArg;
		DetectFlags m_detect;

		ThreadData m_threadData[THREAD_MAX];
		hkCriticalSection m_threadDataCriticalSection;

	private:

		ThreadData& newThreadData();
};

#endif // HKBASE_hkOptimizerMemorySystem_H

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
