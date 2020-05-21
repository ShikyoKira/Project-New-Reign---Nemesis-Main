/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MEMORY_EXCEPTION_TESTING_UTIL_H
#define HK_MEMORY_EXCEPTION_TESTING_UTIL_H

#include <Common/Base/System/Io/Socket/hkSocket.h>
#include <Common/Base/Object/hkSingleton.h>
#include <Common/Base/DebugUtil/DeterminismUtil/hkCheckDeterminismUtil.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>

//#define HK_ENABLE_MEMORY_EXCEPTION_UTIL
extern hkMemoryAllocator::MemoryState hkOutOfMemoryState;

void hkSetOutOfMemoryState( hkMemoryAllocator::MemoryState state );

HK_FORCE_INLINE hkMemoryAllocator::MemoryState hkGetOutOfMemoryState()
{
	return  hkOutOfMemoryState;
}

#if ! defined (HK_ENABLE_MEMORY_EXCEPTION_UTIL)
HK_FORCE_INLINE bool hkMemoryStateIsOutOfMemory(int id)
{
	bool lowOnMem = hkGetOutOfMemoryState() == hkMemoryAllocator::MEMORY_STATE_OUT_OF_MEMORY;
	return lowOnMem;
}

HK_FORCE_INLINE bool hkHasMemoryAvailable(int id, int size)
{
	bool hasAvailable = hkMemorySystem::getInstance().heapCanAllocTotal(size);
	return hasAvailable;
}
#else
HK_FORCE_INLINE bool hkMemoryStateIsOutOfMemory(int id)
{
	if ( !hkMemoryExceptionTestingUtil::isMemoryAvailable(id) )
	{
		return true;
	}
	bool lowOnMem = hkGetOutOfMemoryState() == hkMemoryAllocator::MEMORY_STATE_OUT_OF_MEMORY;
	return lowOnMem;
}

HK_FORCE_INLINE bool hkHasMemoryAvailable(int id, int size)
{
	if ( !hkMemoryExceptionTestingUtil::isMemoryAvailable(id) )
	{
		return false;
	}
	bool hasAvailable = hkMemorySystem::getInstance().heapCanAllocTotal(size);
	return hasAvailable;
}
#endif

class hkMemoryExceptionTestingUtil
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkMemoryExceptionTestingUtil );
		hkMemoryExceptionTestingUtil();
		~hkMemoryExceptionTestingUtil();

		static void HK_CALL create();
		static void HK_CALL destroy();
		static hkMemoryExceptionTestingUtil& HK_CALL getInstance();

		static void HK_CALL startNewDemo() { if (s_instance) s_instance->startNewDemoImpl(); }
		static void HK_CALL startFrame()   { if (s_instance) s_instance->startFrameImpl(); }
		static void HK_CALL endFrame()     { if (s_instance) s_instance->endFrameImpl(); }

		static void HK_CALL allowMemoryExceptions(bool allowMemExceptions) { if (s_instance) s_instance->allowMemoryExceptionsImpl(allowMemExceptions); }

		static bool HK_CALL isMemoryAvailable(int id) { if (s_instance) return s_instance->isMemoryAvailableImpl(id); return true; }


		void startNewDemoImpl();
		void startFrameImpl();
		void endFrameImpl();

		void allowMemoryExceptionsImpl(bool allowMemExceptions);

		bool isMemoryAvailableImpl(int id);

		static hkMemoryExceptionTestingUtil* s_instance;

		bool m_outOfMemory;

		int m_frameCounter;
		bool m_allowMemoryExceptions;

		enum{ MAX_CHECK_ID = 50};
		bool m_wasCheckIdThrown[MAX_CHECK_ID];
};

#endif // HK_MEMORY_EXCEPTION_TESTING_UTIL_H

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
