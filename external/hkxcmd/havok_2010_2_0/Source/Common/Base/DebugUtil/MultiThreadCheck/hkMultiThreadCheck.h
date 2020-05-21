/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_MULTI_THREADING_TYPES_H
#define HK_DYNAMICS2_MULTI_THREADING_TYPES_H


extern const class hkClass hkMultiThreadCheckClass;

#if defined (HK_DEBUG) && !defined(HK_PLATFORM_SPU)
	/// Enabling multi threading debugging. This is currently only done in debug mode.
	/// If you use a debug build and link against release libraries, everything should work fine except you do not get multithreaded debugging.
	/// If you use a release build and link against debug libraries, you have to enable this define in order to avoid the asserts
#	define HK_DEBUG_MULTI_THREADING
#endif

#ifdef HK_DEBUG_MULTI_THREADING
#	define HK_ON_DEBUG_MULTI_THREADING(x) x
#else
#	define HK_ON_DEBUG_MULTI_THREADING(x)
#endif

#include <Common/Base/System/StackTracer/hkStackTracer.h>

	/// A structure used in hkpWorld to provide debug access checking.
class hkMultiThreadCheck
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_WORLD, hkMultiThreadCheck);
		HK_DECLARE_REFLECTION();

		enum AccessType
		{
			HK_ACCESS_IGNORE = 0,
			HK_ACCESS_RO     = 1,
			HK_ACCESS_RW     = 2,
		};

		HK_FORCE_INLINE hkMultiThreadCheck();

		HK_FORCE_INLINE void init();

		enum ReadMode
		{
			THIS_OBJECT_ONLY,
			RECURSIVE
		};

			/// Mark this class and (if mode = RECURSUVE all child classes)
			/// for read only access for this thread
			/// Note: This is only for debugging and does not wait to get exclusive access,
			/// but simply assert if another thread marked the hkpWorld. You must read the
			/// user guide about multithreading to use this.
		HK_FORCE_INLINE void markForRead( ReadMode mode = RECURSIVE )
		{
			HK_ON_DEBUG_MULTI_THREADING( markForReadImpl( mode ) );
		}

			/// Mark this class and all child classes for read write access for this thread
			/// Note: This is only for debugging and does not wait to get exclusive access,
			/// but simply assert if another thread marked the hkpWorld. You must read the
			/// user guide about multithreading to use this.
		HK_FORCE_INLINE void markForWrite( )
		{
			HK_ON_DEBUG_MULTI_THREADING( markForWriteImpl() );
		}

			/// Returns true, if this class already has write access
		HK_FORCE_INLINE bool isMarkedForWrite( )
		{
#ifdef HK_DEBUG_MULTI_THREADING
			return isMarkedForWriteImpl();
#else
			return true;
#endif
		}

		HK_FORCE_INLINE bool isMarkedForReadRecursive()
		{
#ifdef HK_DEBUG_MULTI_THREADING
			return ( isCheckingEnabled() && ( hkUint32(MARKED_RO) == m_threadId ) );
#else
			return true;
#endif
		}

			/// Undo markForRead
			/// Note: This is only for debugging and does not wait to get exclusive access,
			/// but simply assert if another thread marked the hkpWorld. You must read the
			/// user guide about multithreading to use this.
		HK_FORCE_INLINE void unmarkForRead()
		{
			HK_ON_DEBUG_MULTI_THREADING( unmarkForReadImpl() );
		}

			/// Unmark For write
			/// Note: This is only for debugging and does not wait to get exclusive access,
			/// but simply assert if another thread marked the hkpWorld. You must read the
			/// user guide about multithreading to use this.
		HK_FORCE_INLINE void unmarkForWrite()
		{
			HK_ON_DEBUG_MULTI_THREADING( unmarkForWriteImpl() );
		}

			/// Disable checks
		void disableChecks();

			/// Re-enables checks.
		void enableChecks();

			
			/// Re-enables checks without resetting data to default values
		void reenableChecks();

			/// Get whether checking is enabled
		inline bool isCheckingEnabled() const
		{
			
			return ( ( 0x8000 & m_markCount ) == 0 && m_criticalSection );
		}

		void accessCheck( AccessType type ) const;

		static void HK_CALL accessCheckWithParent( const hkMultiThreadCheck* parentLock, AccessType parentType, const hkMultiThreadCheck& lock, AccessType type );

			/// call this to enable this utility
		static void staticInit(hkMemoryAllocator *memoryAllocator);

			/// call this to disable this utility
			/// calling this function is not necessary, you will just get a small memory leak
		static void staticQuit();

	protected:
		void markForReadImpl( ReadMode mode );
		void markForWriteImpl( );
		bool isMarkedForWriteImpl( );
		void unmarkForReadImpl( );
		void unmarkForWriteImpl();

	public:
		enum
		{
			MARKED_RO = 0xffffffe1,	// and all children
			MARKED_RO_SELF_ONLY = 0xffffffc1,
			UNMARKED = 0xfffffff1
		};

			/// Stored in a 64bit property anyway, and in the sim islands (not serialized, so we can use more than the previous 8 bits)
			/// If the threadid is bigger we can catch similar ids better (the id is not an index but the actual platform id)
		hkUint32 m_threadId; //+nosave

			
			/// Traces the call stack when markForWrite() is called. In the event of a conflicting lock, the stack trace can be displayed
			/// to aid in debugging.
		static hkStackTracer* s_stackTracer;

			/// Tree in which to store stack traces. Each unique call stack from which markForWrite() was called will be inserted.
		static hkStackTracer::CallTree* s_stackTree;

			/// Identifies the location in the stack trace tree which contains the most recent stack trace from which markForWrite() was called for
			/// this hkMultiThreadCheck.
		int m_stackTraceId; //+nosave

			/// The number of marks on the mark bit stack.
			/// Access checking is marked as disabled by setting the highest bit in this variable.
			/// The mark count is accessed only when this bit is not set.
		hkUint16 m_markCount; //+nosave


  protected:

			/// This is a 16 bit unsigned integer that stores the state of any nested
			/// marks. If you are nesting read marks inside write marks, this stores
			/// the order that the marks were placed, to ensure symmetric calls to
			/// markForRead and markForWrite. A set bit indicates a write mark and an
			/// unset bit indicates a read mark. The top of the stack is always the
			/// least significant bit. The size of the stack is always equal to the
			/// current value of 'm_markCount'.
		hkUint16 m_markBitStack; //+nosave

	public:
		static class hkCriticalSection* m_criticalSection;

			/// Lock the critical section to avoid contention when marking a large number of
			/// objects for read or write
		static void globalCriticalSectionLock();

		static void globalCriticalSectionUnlock();
};


#ifdef HK_DEBUG_MULTI_THREADING
#	define HK_ACCESS_CHECK_WITH_PARENT( parent, parentAccess, object, objectAccess ) hkMultiThreadCheck::accessCheckWithParent( (parent)? &(parent)->getMultiThreadCheck():HK_NULL, hkMultiThreadCheck:: parentAccess, object->getMultiThreadCheck(), hkMultiThreadCheck:: objectAccess )
#	define HK_ACCESS_CHECK_OBJECT( object, objectAccess ) if ( object ){ object->m_multiThreadCheck.accessCheck( hkMultiThreadCheck:: objectAccess ); }

	hkMultiThreadCheck::hkMultiThreadCheck(): m_threadId( (hkUint32)UNMARKED ), m_markCount(0) {}
	void hkMultiThreadCheck::init(){ m_threadId = (hkUint32)UNMARKED; m_markCount = 0; }

#else
#	define HK_ACCESS_CHECK_WITH_PARENT( parent, parentAccess, object, objectAccess ) 
#	define HK_ACCESS_CHECK_OBJECT( object, objectAccess ) 
	hkMultiThreadCheck::hkMultiThreadCheck(): m_threadId( (hkUint32)UNMARKED ), m_markCount( 0x8000 ){}
	void hkMultiThreadCheck::init(){ m_threadId = (hkUint32)UNMARKED; m_markCount = 0x8000; }
#endif


#endif // HK_DYNAMICS2_MULTI_THREADING_TYPES_H

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
