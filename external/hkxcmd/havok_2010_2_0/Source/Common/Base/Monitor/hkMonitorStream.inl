/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#define HK_PERF_BOOKMARK( VAL )

#if defined(HK_PLATFORM_X64)
extern "C" {
	unsigned __int64 __rdtsc (void);
	#pragma intrinsic(__rdtsc)
}
#elif defined(HK_PLATFORM_XBOX360)


	extern "C" { unsigned __int64 __mftb(); }
	#pragma intrinsic(__mftb)

//#define HK_XBOX_USE_PERFLIB // Only for internal testing
#ifdef HK_XBOX_USE_PERFLIB
#include <Common/Base/Monitor/XBox360/hkXBox360MonitorStream.inl>
#endif

#endif

#ifndef __GCCXML__
#ifdef HK_PLATFORM_MACARM
#	include <mach/mach_time.h>
#elif defined(HK_PLATFORM_MACPPC) || defined(HK_PLATFORM_MAC386)
#	include <Kernel/mach/mach_time.h>
#endif
#endif

void hkMonitorStream::TimerCommand::setTime()
{
#if defined(HK_ARCH_IA32)
		TimerValue ticks;
		__asm {
			//push ebx
			//cpuid
			//pop ebx
			rdtsc
			mov dword ptr[ticks], eax
#		ifdef HK_ENABLE_64BIT_TIMERS
			mov dword ptr[ticks+4], edx
#		endif
		}
		m_time0 = ticks;

#elif defined( HK_PLATFORM_PSP )

// fill in with the correct performance counter info when available
// for now we will use the system timing data ( accurate to 1 microsecond )
	m_time0 = sceKernelGetSystemTimeLow();

#elif defined(HK_PLATFORM_GC)
#elif defined(HK_PLATFORM_XBOX360)

	m_time0 = (hkUint32)__mftb();
#	ifdef  HK_XBOX_USE_PERFLIB
		m_time1 = HK_HvxGetPRV32FromAddr(g_hkXbox360PerfSampleRegAddr);
#	endif
#elif defined(HK_ARCH_X64)
	#if defined(HK_PLATFORM_X64)
		m_time0 = static_cast<hkUint32>( __rdtsc() );
#	else
		hkUint32 junk;
		__asm__ __volatile__ (	"rdtsc\n\t"
								: "=a" (m_time0), "=d" (junk));
#	endif
#elif defined(HK_PLATFORM_PS3_PPU)
		asm volatile ("mftb %0" : "=r"(m_time0));
#elif defined(HK_PLATFORM_PS3_SPU)
	m_time0 = -spu_readch(SPU_RdDec); // negative, as a decrementer
#elif !defined(__GCCXML__) && (defined(HK_PLATFORM_MACARM) || defined(HK_PLATFORM_MAC386) || defined(HK_PLATFORM_MACPPC))
    m_time0 = (hkUint64) mach_absolute_time();
    m_time1 = 0;
#else
	m_time0 = 0;
//#	error dont know how to set time
#endif
}


#define HK_MONITOR_TYPE_TIMER "t"
#define HK_MONITOR_TYPE_SINGLE "f"
#define HK_MONITOR_TYPE_INT "i"

#define HK_MONITOR_COMMAND_PUSH_DIR "Pd"
#define HK_MONITOR_COMMAND_POP_DIR "pd"
#define HK_MONITOR_COMMAND_BEGIN_TIMER "T"
#define HK_MONITOR_COMMAND_BEGIN_OBJECT_NAME "O"

#define HK_MONITOR_COMMAND_END_TIMER "E"
#define HK_MONITOR_COMMAND_BEGIN_LIST "L"
#define HK_MONITOR_COMMAND_SPLIT_LIST "S"
#define HK_MONITOR_COMMAND_END_LIST "l"

#define HK_MONITOR_COMMAND_ADD_VALUE "M"

//#define HK_MONITOR_COMMAND_MEMORY_* "m" // m* tags reserved for memory

#define HK_MONITOR_COMMAND_NOP "N" // nops added when monitor stream transfers have to be of certain alignment (eg: on SPUs)

#if HK_CONFIG_MONITORS == HK_CONFIG_MONITORS_ENABLED

#	define HK_ON_MONITORS_ENABLED( code ) code

#	define HK_MONITOR_ADD_VALUE( NAME, VALUE, MONITOR_TYPE ){   \
		hkMonitorStream& mStream = hkMonitorStream::getInstance();		\
		if ( mStream.memoryAvailable() )							\
		{																	\
			 hkMonitorStream::AddValueCommand* h = reinterpret_cast<hkMonitorStream::AddValueCommand*>(mStream.getEnd());	\
			 h->m_commandAndMonitor = HK_MONITOR_COMMAND_ADD_VALUE MONITOR_TYPE NAME;				\
			 h->m_value = VALUE;											\
			 mStream.setEnd( (char*)(h+1) );						\
		}	}

#	define HK_TIMER_INTERN( STREAM, NAME, COMMAND, OBJECT )	{						\
		if ( STREAM.memoryAvailable() )										\
		{																	\
			hkMonitorStream::TimerCommand* h = reinterpret_cast<hkMonitorStream::TimerCommand*>(STREAM.getEnd());	\
			 h->m_commandAndMonitor = COMMAND HK_MONITOR_TYPE_TIMER NAME;	\
			 h->setTime();													\
			 STREAM.setEnd( (char*)(h+1) );						\
		} }

#	define HK_TIMER_INTERN_LIST( STREAM, NAME1, NAME2, COMMAND, OBJECT ) {			\
		if ( STREAM.memoryAvailable() )										\
		{																	\
			hkMonitorStream::TimerBeginListCommand* h = reinterpret_cast<hkMonitorStream::TimerBeginListCommand*>(STREAM.getEnd());	\
			 h->m_commandAndMonitor = COMMAND HK_MONITOR_TYPE_TIMER NAME1;	\
			 h->m_nameOfFirstSplit = HK_MONITOR_COMMAND_SPLIT_LIST HK_MONITOR_TYPE_TIMER NAME2; \
			 h->setTime();													\
			 STREAM.setEnd( (char*)(h+1) );						\
		} }

#	define HK_TIMER_BEGIN_OBJECT_NAME(STREAM, OBJECT_NAME) {				\
	if ( STREAM.memoryAvailable() )										\
	{																	\
		hkMonitorStream::TimerBeginObjectNameCommand* h = reinterpret_cast<hkMonitorStream::TimerBeginObjectNameCommand*>(STREAM.getEnd());	\
		h->m_commandAndMonitor = HK_MONITOR_COMMAND_BEGIN_OBJECT_NAME HK_MONITOR_TYPE_TIMER;	\
		h->m_objectName = OBJECT_NAME; \
		h->setTime();													\
		STREAM.setEnd( (char*)(h+1) );						\
	} }

#	define HK_MONITOR_MEMORY_INTERNAL(STREAM, TAG, PTR, NBYTES) { \
	if ( STREAM.memoryAvailable() ) \
		{ \
		hkMonitorStream::MemoryCommand* h = reinterpret_cast<hkMonitorStream::MemoryCommand*>(STREAM.getEnd()); \
		h->m_commandAndMonitor = TAG; \
		h->m_ptr = PTR; \
		h->m_sizeAndFlags = NBYTES; \
		STREAM.setEnd( (char*)(h+1) );	\
} }


#	define HK_MONITOR_MEMORY_ALLOC(TAG, PTR, NBYTES) { 	hkMonitorStream& mStream = hkMonitorStream::getInstance(); HK_MONITOR_MEMORY_INTERNAL(mStream, TAG,PTR,NBYTES); }
#	define HK_MONITOR_MEMORY_FREE(TAG, PTR, NBYTES)  { 	hkMonitorStream& mStream = hkMonitorStream::getInstance(); HK_MONITOR_MEMORY_INTERNAL(mStream, TAG,PTR,(NBYTES)|(1<<31)); }

#	define HK_TIMER_BEGIN( NAME, OBJECT )	{ 	hkMonitorStream& mStream = hkMonitorStream::getInstance(); HK_TIMER_INTERN( mStream, NAME, HK_MONITOR_COMMAND_BEGIN_TIMER, OBJECT	); }
#	define HK_TIMER_END(  )					{ 	hkMonitorStream& mStream = hkMonitorStream::getInstance(); HK_TIMER_INTERN( mStream, "", HK_MONITOR_COMMAND_END_TIMER, HK_NULL	); }

	/// Timer end call which also checks for matching timer begin call
#	define HK_TIMER_NAMED_END( NAME )		{ 	hkMonitorStream& mStream = hkMonitorStream::getInstance(); HK_TIMER_INTERN( mStream, NAME, HK_MONITOR_COMMAND_END_TIMER, HK_NULL	); }

#	define HK_TIMER_BEGIN_LIST( NAME, FIRSTITEM )	{ 	hkMonitorStream& mStream = hkMonitorStream::getInstance(); HK_TIMER_INTERN_LIST( mStream, NAME, FIRSTITEM, HK_MONITOR_COMMAND_BEGIN_LIST, HK_NULL	);}
#	define HK_TIMER_SPLIT_LIST( NEXTITEM )			{ 	hkMonitorStream& mStream = hkMonitorStream::getInstance(); HK_TIMER_INTERN( mStream, NEXTITEM, HK_MONITOR_COMMAND_SPLIT_LIST, HK_NULL	);}
#	define HK_TIMER_END_LIST( )						{ 	hkMonitorStream& mStream = hkMonitorStream::getInstance(); HK_TIMER_INTERN( mStream, "", HK_MONITOR_COMMAND_END_LIST, HK_NULL	);}


	//
	//	Timer calls where you pass on a stream to avoid tls lookups
	//
#	define HK_TIMER_BEGIN2( STREAM, NAME, OBJECT )			HK_TIMER_INTERN( STREAM, NAME, HK_MONITOR_COMMAND_BEGIN_TIMER, OBJECT	)
#	define HK_TIMER_END2( STREAM )							HK_TIMER_INTERN( STREAM, "", HK_MONITOR_COMMAND_END_TIMER, HK_NULL	)

	/// Timer end call which also checks for matching timer begin call
#	define HK_TIMER_NAMED_END2( STREAM, NAME )				HK_TIMER_INTERN( STREAM, NAME, HK_MONITOR_COMMAND_END_TIMER, HK_NULL	)

#	define HK_TIMER_BEGIN_LIST2( STREAM, NAME, FIRSTITEM )	HK_TIMER_INTERN_LIST( STREAM, NAME, FIRSTITEM, HK_MONITOR_COMMAND_BEGIN_LIST, HK_NULL	)
#	define HK_TIMER_SPLIT_LIST2( STREAM, NEXTITEM )			HK_TIMER_INTERN( STREAM, NEXTITEM, HK_MONITOR_COMMAND_SPLIT_LIST, HK_NULL	)
#	define HK_TIMER_END_LIST2( STREAM )						HK_TIMER_INTERN( STREAM, "", HK_MONITOR_COMMAND_END_LIST, HK_NULL	)

#if !defined(__HAVOK_PARSER__) && (defined(HK_COMPILER_GCC) || defined(HK_COMPILER_SNC))
#	define HK_POSSIBLY_UNUSED __attribute__((unused))
#else
#	define HK_POSSIBLY_UNUSED
#endif

class HK_POSSIBLY_UNUSED hkTimeFunctionHelper
{
	public:
		HK_FORCE_INLINE ~hkTimeFunctionHelper() { HK_TIMER_END(); }
};

#define HK_TIME_CODE_BLOCK( NAME, OBJECT ) HK_TIMER_BEGIN(NAME, OBJECT); hkTimeFunctionHelper HK_PREPROCESSOR_JOIN_TOKEN(timeblock, __LINE__)

#	define HK_MONITOR_PUSH_DIR( PATH )	{							\
	hkMonitorStream& mStream = hkMonitorStream::getInstance();		\
	if ( mStream.memoryAvailable() )									\
	{																\
		 hkMonitorStream::Command* h = (hkMonitorStream::Command*)( mStream.expandby( sizeof(hkMonitorStream::Command) ) );	\
		 h->m_commandAndMonitor = HK_MONITOR_COMMAND_PUSH_DIR PATH;	\
	} }

#	define HK_MONITOR_POP_DIR(  )	{								\
	hkMonitorStream& mStream = hkMonitorStream::getInstance();		\
	if ( mStream.memoryAvailable() )									\
	{																\
		 hkMonitorStream::Command* h = (hkMonitorStream::Command*)( mStream.expandby( sizeof(hkMonitorStream::Command) ) );	\
		 h->m_commandAndMonitor = HK_MONITOR_COMMAND_POP_DIR;		\
	}	}

#	define HK_MONITOR_NOP()	{										\
	hkMonitorStream& mStream = hkMonitorStream::getInstance();		\
	if ( mStream.getEnd() <= (mStream.getCapacity() - sizeof(hkMonitorStream::Command)) )									\
	{																\
		hkMonitorStream::Command* h = (hkMonitorStream::Command*)( mStream.expandby( sizeof(hkMonitorStream::Command) ) );	\
		h->m_commandAndMonitor = HK_MONITOR_COMMAND_NOP;	\
	}	}

#else // HK_CONFIG_MONITORS == HK_CONFIG_MONITORS_DISABLED

#	define HK_ON_MONITORS_ENABLED( code )

#	define HK_MONITOR_PUSH_DIR( PATH )
#	define HK_MONITOR_POP_DIR()
#	define HK_MONITOR_ADD_VALUE( NAME, VALUE, MONITOR_TYPE )
#	define HK_MONITOR_NOP()
#	define HK_MONITOR_MEMORY_ALLOC(TAG, PTR, NBYTES)
#	define HK_MONITOR_MEMORY_FREE(TAG, PTR, NBYTES)

#	define HK_TIMER_BEGIN( NAME, OBJECT )
#	define HK_TIMER_BEGIN_OBJECT_NAME( STREAM, OBJECT_NAME )
#	define HK_TIMER_END()
#	define HK_TIMER_NAMED_END(NAME)
#	define HK_TIMER_BEGIN_LIST( NAME, OBJECT )
#	define HK_TIMER_SPLIT_LIST( NAME )
#	define HK_TIMER_END_LIST(  )

#	define HK_TIMER_BEGIN2( STREAM, NAME, OBJECT )
#	define HK_TIMER_END2( STREAM )
#	define HK_TIMER_NAMED_END2( STREAM, NAME )
#	define HK_TIMER_BEGIN_LIST2( STREAM, NAME, FIRSTITEM )
#	define HK_TIMER_SPLIT_LIST2( STREAM, NEXTITEM )
#	define HK_TIMER_END_LIST2( STREAM )

#define HK_TIME_CODE_BLOCK( NAME, OBJECT )

#endif


// Set this to 1 for extra detailed timers

#	define HK_INTERNAL_TIMER_BEGIN(NAME,OBJECT)		{}
#	define HK_INTERNAL_TIMER_END()				{}
#	define HK_INTERNAL_TIMER_BEGIN_LIST(a,b)	{}
#	define HK_INTERNAL_TIMER_SPLIT_LIST(a)		{}
#	define HK_INTERNAL_TIMER_END_LIST()		{}
#	define HK_INTERNAL_MONITOR_ADD_VALUE(NAME, VALUE, MONITOR_TYPE) {}
#	define HK_INTERNAL_TIME_CODE_BLOCK( NAME, OBJECT) {}

#	define HK_USER_TIMER_BEGIN(NAME,OBJECT)		HK_TIMER_BEGIN(NAME,HK_NULL)
#	define HK_USER_TIMER_END				HK_TIMER_END
#	define HK_USER_TIMER_BEGIN_LIST(a,b)	HK_TIMER_BEGIN_LIST(a,b)
#	define HK_USER_TIMER_SPLIT_LIST(a)		HK_TIMER_SPLIT_LIST(a)
#	define HK_USER_TIMER_END_LIST			HK_TIMER_END_LIST

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
