/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_hkMemoryRouter_H
#define HKBASE_hkMemoryRouter_H

#include <Common/Base/Config/hkConfigThread.h>
#include <Common/Base/Thread/Thread/hkThreadLocalData.h>
#include <Common/Base/Memory/Allocator/hkMemoryAllocator.h>
#include <Common/Base/Memory/Allocator/Lifo/hkLifoAllocator.h>

class hkLifoAllocator;

#if defined(HK_DEBUG_SPU) || defined(HK_DEBUG)
#	define HK_ERROR(a,b) if(1) { HK_BREAKPOINT(0); } else 
#	define HK_ASSERT2(a,b,c) if(!(b)) { HK_BREAKPOINT(0); } else
#	define HK_CRITICAL_ASSERT2(a,b,c) if(!(b)) { HK_BREAKPOINT(0); } else
#else
#	define HK_ERROR(a,b)
#	define HK_ASSERT2(a,b,c)
#	define HK_CRITICAL_ASSERT2(a,b,c)
#endif


#undef HK_CRITICAL_ASSERT2
#undef HK_ASSERT2
#undef HK_ERROR

class hkOstream;

	/// All Havok allocations are routed through hkMemoryRouter.
	/// Generally, each thread has a thread-local hkMemoryRouter instance which
	/// is initialized by the hkMemorySystem.
class hkMemoryRouter
{
	//+hk.MemoryTracker(ignore=True)
	public:
		HK_DECLARE_PLACEMENT_ALLOCATOR();

		typedef hkLifoAllocator Stack;

		typedef hkMemoryAllocator Allocator;

			/// Set the thread-local instance pointer to the supplied router.
			/// Note that instances are not ref counted and the caller is responsible for cleanup.
		static void HK_CALL replaceInstance( hkMemoryRouter* m );

			/// Access the thread-local instance.
		static inline hkMemoryRouter& HK_CALL getInstance();

			/// Access the thread-local instance.
		static inline hkMemoryRouter* HK_CALL getInstancePtr();

			/// Construct an empty object.
		hkMemoryRouter();

			///
		void resetPeakMemoryStatistics();

			/// Small thread local allocation in a stack pattern.
		inline Stack& stack() { return m_stack; }

			/// Temporary allocation, thread local. Often but not always in a stack pattern.
		inline Allocator& temp() { return *m_temp; }
			///
		inline void setTemp(Allocator* a) { m_temp = a; }

			/// Allocation which is likely to persist. Not thread local.
		inline Allocator& heap() { return *m_heap; }
			///
		inline void setHeap(Allocator* a) { m_heap = a; }

			/// Allocation which may break the rules. Not thread local.
		inline Allocator& debug() { return *m_debug; }
			///
		inline void setDebug(Allocator* a) { m_debug = a; }

			/// Allocator for the constraint solver. These may request large contiguous and frame local blocks.
		inline Allocator& solver() { return *m_solver; }
			///
		inline void setSolver(Allocator* a) { m_solver = a; }

			/// User data pointer, often used by the hkMemorySystem.
		inline void* getUserData() const { return m_userData; }
			///
		inline void setUserData(void* p) { m_userData = p; }

			/// Helper to align an allocation request, note that the corresponding free method must be used.
		static void* HK_CALL alignedAlloc( Allocator& b, int nbytes, int align );

			/// Free a block allocated with alignedAlloc.
		static void HK_CALL alignedFree( Allocator& b, void* p );

			/// Helper to make an allocation which remembers the size internally.
		static void* HK_CALL easyAlloc( Allocator& b, int nbytes );
			/// Returns the size of an 'easyAlloc' allocation.
		static hk_size_t HK_CALL getEasyAllocSize(Allocator& b, const void* ptr);
			/// Get the start address of the 'easyAlloc' allocation.
		static const void* HK_CALL getEasyAllocStartAddress(Allocator& b, const void* ptr);

			/// Free a block allocated with easyAlloc.
		static void HK_CALL easyFree( Allocator& b, void* p );

	protected:

		Stack m_stack;

		Allocator* m_temp;		///< frame local allocations
		Allocator* m_heap;	///< longer than frame local
		Allocator* m_debug;		///< monitors, debugmem,
		Allocator* m_solver;	///< special frame local allocator for solver buffer allocation of Havok Physics, check out hkSolverAllocator for details
		void* m_userData;

		static HK_THREAD_LOCAL(hkMemoryRouter*) s_memoryRouter;

};

inline hkMemoryRouter& HK_CALL hkMemoryRouter::getInstance()
{
	return *HK_THREAD_LOCAL_GET(s_memoryRouter);
}
inline hkMemoryRouter* HK_CALL hkMemoryRouter::getInstancePtr()
{
	return HK_THREAD_LOCAL_GET(s_memoryRouter);
}


#if !defined(HK_PLATFORM_SIM) && (defined(__GCCXML__) || defined(HK_DEBUG))
	#define HK_MEMORY_TRACKER_ENABLE
#endif

#if defined(HK_MEMORY_TRACKER_ENABLE)
#include <Common/Base/Memory/Tracker/hkMemoryTracker.h>
#include <Common/Base/Memory/Tracker/CurrentFunction/hkCurrentFunction.h>

	#define HK_MEMORY_TRACKER_ON_NEW_REFOBJECT(NBYTES, OBJ) if( hkMemoryTracker* _trk = hkMemoryTracker::getInstancePtr() ) _trk->onNewReferencedObject(getTypeIdentifier(), NBYTES, OBJ)
	#define HK_MEMORY_TRACKER_ON_DELETE_REFOBJECT(OBJ) if( hkMemoryTracker* _trk = hkMemoryTracker::getInstancePtr() ) _trk->onDeleteReferencedObject(OBJ)
	#define HK_MEMORY_TRACKER_ON_NEW_OBJECT(CLASS_TYPE, NBYTES, OBJ) if( hkMemoryTracker* _trk = hkMemoryTracker::getInstancePtr() ) _trk->onNewObject(getTypeIdentifier(), NBYTES, OBJ);
	#define HK_MEMORY_TRACKER_ON_DELETE_OBJECT(OBJ) if( hkMemoryTracker* _trk = hkMemoryTracker::getInstancePtr() ) _trk->onDeleteObject(OBJ);
	#define HK_DECLARE_MEMORY_TRACKER_HOOK() \
		public: struct TrackerStruct; \
		HK_FORCE_INLINE static const char* getTypeIdentifier() { return HK_METHOD_CLASS_NAME; }

	// When an object is created or destroyed, if the tracker isn't informed thru new/delete methods, use the following macros to inform the tracker

	#define HK_MEMORY_TRACKER_NEW_REFOBJECT(TYPE_NAME, OBJ) { if( hkMemoryTracker* _trk = hkMemoryTracker::getInstancePtr() ) _trk->onNewReferencedObject("!" #TYPE_NAME, sizeof(TYPE_NAME), OBJ); }
	#define HK_MEMORY_TRACKER_NEW_SIZED_REFOBJECT(TYPE_NAME, OBJ, SIZE) { if( hkMemoryTracker* _trk = hkMemoryTracker::getInstancePtr() ) _trk->onNewReferencedObject("!" #TYPE_NAME, SIZE, OBJ); }
	#define HK_MEMORY_TRACKER_DELETE_REFOBJECT(OBJ) { if( hkMemoryTracker* _trk = hkMemoryTracker::getInstancePtr() ) _trk->onDeleteReferencedObject(OBJ); }
	#define HK_MEMORY_TRACKER_NEW_OBJECT(TYPE_NAME, OBJ) { if( hkMemoryTracker* _trk = hkMemoryTracker::getInstancePtr() ) _trk->onNewObject("!" #TYPE_NAME, sizeof(TYPE_NAME), OBJ); }
	#define HK_MEMORY_TRACKER_DELETE_OBJECT(OBJ) { if( hkMemoryTracker* _trk = hkMemoryTracker::getInstancePtr() ) _trk->onDeleteObject(OBJ); }

#else
	#define HK_MEMORY_TRACKER_ON_NEW_REFOBJECT(NBYTES, OBJ)
	#define HK_MEMORY_TRACKER_ON_DELETE_REFOBJECT(OBJ)
	#define HK_MEMORY_TRACKER_ON_NEW_OBJECT(CLASS_TYPE, NBYTES, OBJ)
	#define HK_MEMORY_TRACKER_ON_DELETE_OBJECT(OBJ)
	#define HK_DECLARE_MEMORY_TRACKER_HOOK()

	#define HK_MEMORY_TRACKER_NEW_REFOBJECT(TYPE_NAME, OBJ) 
	#define HK_MEMORY_TRACKER_NEW_SIZED_REFOBJECT(TYPE_NAME, OBJ, SIZE)
	#define HK_MEMORY_TRACKER_DELETE_REFOBJECT(OBJ) 
	#define HK_MEMORY_TRACKER_NEW_OBJECT(TYPE_NAME, OBJ) 
	#define HK_MEMORY_TRACKER_DELETE_OBJECT(OBJ) 

#endif

#	define HK_OPERATOR_DELETE \
		HK_FORCE_INLINE void  HK_CALL operator delete(void*, void*)		{ }	\
		HK_FORCE_INLINE void  HK_CALL operator delete[](void*, void*)	{ HK_BREAKPOINT(0);	}
#	define HK_OPERATOR_NONVIRTUAL_DELETE \
		HK_FORCE_INLINE void  HK_CALL operator delete(void*, void*)		{ } \
		HK_FORCE_INLINE void  HK_CALL operator delete[](void*, void*)	{ }

#if (defined(HK_COMPILER_GCC)) || defined(__GCCXML__)
#	define HK_MUST_END_WITH_SEMICOLON
#else
#	define HK_MUST_END_WITH_SEMICOLON class MustEndWithSemiColon
#endif

#	define HK_DECLARE_CLASS_ALLOCATOR_UNCHECKED(MEMORY_CLASS, ALLOCATOR) \
		HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t nbytes) { \
			HK_ASSERT_OBJECT_SIZE_OK(nbytes); \
			hkReferencedObject* b = static_cast<hkReferencedObject*>(hkMemoryRouter::getInstance().ALLOCATOR().blockAlloc(static_cast<int>(nbytes))); \
			b->m_memSizeAndFlags = static_cast<hkUint16>(nbytes); \
			HK_MEMORY_TRACKER_ON_NEW_REFOBJECT(nbytes, b); \
			return b; }	\
		HK_FORCE_INLINE void  HK_CALL operator delete(void* p) { \
			hkReferencedObject* b = static_cast<hkReferencedObject*>(p); \
			HK_MEMORY_TRACKER_ON_DELETE_REFOBJECT(b); \
			hkMemoryRouter::getInstance().ALLOCATOR().blockFree(p, b->m_memSizeAndFlags); }	\
		HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t, void* p)	{ return p; }	\
		HK_FORCE_INLINE void* HK_CALL operator new[](hk_size_t, void* p){ HK_BREAKPOINT(0); return p; }	\
		HK_OPERATOR_DELETE \
		HK_DECLARE_MEMORY_TRACKER_HOOK() \
		HK_MUST_END_WITH_SEMICOLON
#	define HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR_BY_SIZE_UNCHECKED(MEMORY_CLASS, CLASS_TYPE, ALLOCATOR) \
		HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t nbytes) { \
			HK_ASSERT2(0x6c787b7f, nbytes == sizeof(CLASS_TYPE), "Incorrect allocation size. Check that the most derived class has an allocator declaration" ); \
			void* obj = hkMemoryRouter::getInstance().ALLOCATOR().blockAlloc(static_cast<int>(nbytes)); \
			HK_MEMORY_TRACKER_ON_NEW_OBJECT(CLASS_TYPE, nbytes, obj); \
			return obj; }	\
		HK_FORCE_INLINE void  HK_CALL operator delete(void* p) { \
			if (p) { \
				HK_MEMORY_TRACKER_ON_DELETE_OBJECT(p); \
				hkMemoryRouter::getInstance().ALLOCATOR().blockFree(p, sizeof(CLASS_TYPE)); \
			} }\
		HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t n, void* p){ HK_ASSERT2(0x77bb90a1, n == sizeof(CLASS_TYPE), "Incorrect allocation size. Check that the most derived class has an allocator declaration"); return p; } \
		HK_FORCE_INLINE void* HK_CALL operator new[](hk_size_t, void* p){ return p;	} \
		HK_OPERATOR_NONVIRTUAL_DELETE \
		HK_DECLARE_MEMORY_TRACKER_HOOK() \
		HK_MUST_END_WITH_SEMICOLON

//
// In debug, use some compile time trickery to ensure correct allocator is used.
// Use -Wshadow to catch nonvirtual allocators in virtual classes.
//
#if defined(HK_DEBUG) || defined(__GCCXML__)
	#define HK_DECLARE_DERIVED_FROM_REFERENCEDOBJECT() \
			HK_FORCE_INLINE const hkReferencedObject* iMustBeDerivedFromReferencedObject() const { return static_cast<const hkReferencedObject*>(this); }
	#define HK_DECLARE_NOT_A_REFERENCEDOBJECT() \
			HK_FORCE_INLINE void iShouldNotHaveVtable() const { int m_memSizeAndFlags = 0; m_memSizeAndFlags--; /* if you get this error, you derive from hkReferencedObject! */ }
#else
	#define HK_DECLARE_DERIVED_FROM_REFERENCEDOBJECT()
	#define HK_DECLARE_NOT_A_REFERENCEDOBJECT()
#endif

// Virtual
#define HK_DECLARE_CLASS_ALLOCATOR(MEMORY_CLASS) \
	HK_DECLARE_DERIVED_FROM_REFERENCEDOBJECT() \
	HK_DECLARE_CLASS_ALLOCATOR_UNCHECKED(MEMORY_CLASS, heap)

#define HK_DECLARE_CLASS_DEBUG_ALLOCATOR(MEMORY_CLASS) \
	HK_DECLARE_DERIVED_FROM_REFERENCEDOBJECT() \
	HK_DECLARE_CLASS_ALLOCATOR_UNCHECKED(MEMORY_CLASS, debug)

// Non-Virtual
#define HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(MEMORY_CLASS, THIS_CLASS) \
	HK_DECLARE_NOT_A_REFERENCEDOBJECT() \
    HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR_BY_SIZE_UNCHECKED(MEMORY_CLASS, THIS_CLASS, heap)

#define HK_DECLARE_NONVIRTUAL_CLASS_DEBUG_ALLOCATOR(MEMORY_CLASS, THIS_CLASS) \
	HK_DECLARE_NOT_A_REFERENCEDOBJECT() \
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR_BY_SIZE_UNCHECKED(MEMORY_CLASS, THIS_CLASS, debug)

// Deprecated functions

	/// Deprecated, use one of the hkMem*Alloc functions.
	template <typename TYPE>
	HK_FORCE_INLINE TYPE* HK_CALL hkAllocate(int numberOfObjects, int cl)
	{
		hkMemoryRouter& a = hkMemoryRouter::getInstance();
		return static_cast<TYPE*>( hkMemoryRouter::easyAlloc( a.heap(), numberOfObjects*hkSizeOfTypeOrVoid<TYPE>::val ));
	}

	/// Convenience inline function to deallocate memory of the correct type
	template <typename TYPE>
	HK_FORCE_INLINE void HK_CALL hkDeallocate(TYPE* ptr)
	{
		hkMemoryRouter& a = hkMemoryRouter::getInstance();
		hkMemoryRouter::easyFree( a.heap(), static_cast<void *>(ptr));
	}

	/// Convenience inline function to allocate aligned memory of the correct type
	template <typename TYPE>
	HK_FORCE_INLINE TYPE* HK_CALL hkAlignedAllocate(int alignment, int numberOfObjects, int cl)
	{
		hkMemoryRouter& a = hkMemoryRouter::getInstance();
		return static_cast<TYPE*>( hkMemoryRouter::alignedAlloc(a.heap(), numberOfObjects*hkSizeOfTypeOrVoid<TYPE>::val, alignment));
	}

	/// Convenience inline function to deallocate memory of the correct type
	template <typename TYPE>
	HK_FORCE_INLINE void HK_CALL hkAlignedDeallocate(TYPE* ptr)
	{
		hkMemoryRouter& a = hkMemoryRouter::getInstance();
		hkMemoryRouter::alignedFree( a.heap(), static_cast<void *>(ptr));
	}

	/// Convenience function to allocate memory of the correct type
	template <typename TYPE>
	HK_FORCE_INLINE TYPE* HK_CALL hkAllocateChunk(int numberOfObjects, int cl)
	{
		return static_cast<TYPE*>(hkMemoryRouter::getInstance().heap().blockAlloc(numberOfObjects*hkSizeOfTypeOrVoid<TYPE>::val));
	}

	/// Convenience function to deallocate memory of the correct type
	template <typename TYPE>
	HK_FORCE_INLINE void HK_CALL hkDeallocateChunk(TYPE* ptr, int numberOfObjects, int cl)
	{
		hkMemoryRouter::getInstance().heap().blockFree(static_cast<void*>(ptr), numberOfObjects*hkSizeOfTypeOrVoid<TYPE>::val);
	}

	

	template <typename TYPE>
	HK_FORCE_INLINE TYPE* HK_CALL hkAllocateStack(int n, const char* what=HK_NULL )
	{
		hkMemoryRouter& mem = hkMemoryRouter::getInstance();
		return static_cast<TYPE*>(mem.stack().fastBlockAlloc(HK_NEXT_MULTIPLE_OF(128,n*hkSizeOfTypeOrVoid<TYPE>::val)));
	}

	template <typename TYPE>
	HK_FORCE_INLINE hkBool32 HK_CALL hkShrinkAllocatedStack(TYPE* ptr, int n)
	{
		return false;
	}

	template <typename TYPE>
	HK_FORCE_INLINE void HK_CALL hkDeallocateStack(TYPE* ptr, int n)
	{
		hkMemoryRouter::getInstance().stack().fastBlockFree(static_cast<void*>(ptr), HK_NEXT_MULTIPLE_OF(128,n*hkSizeOfTypeOrVoid<TYPE>::val));
	}

// end deprecated

#define HK_TEMPLATE_ALLOCATOR(POOL, pool) \
	template <typename TYPE> HK_FORCE_INLINE TYPE* HK_CALL hkMem##POOL##BlockAlloc(int n) \
	{ \
		return hkMemoryRouter::getInstance().pool()._blockAlloc<TYPE>( n ); \
	} \
	template <typename TYPE> HK_FORCE_INLINE void HK_CALL hkMem##POOL##BlockFree( TYPE* t, int n) \
	{ \
		hkMemoryRouter::getInstance().pool()._blockFree<TYPE>( t, n ); \
	} \
	template <typename TYPE> HK_FORCE_INLINE TYPE* HK_CALL hkMem##POOL##BufAlloc(int& reqInOut) \
	{ \
		return hkMemoryRouter::getInstance().pool()._bufAlloc<TYPE>( reqInOut ); \
	} \
	template <typename TYPE> HK_FORCE_INLINE void HK_CALL hkMem##POOL##BufFree( TYPE* t, int n) \
	{ \
		hkMemoryRouter::getInstance().pool()._bufFree<TYPE>( t, n ); \
	} \
	template <typename TYPE> HK_FORCE_INLINE TYPE* HK_CALL hkMem##POOL##BufRealloc( TYPE* t, int nold, int& reqInOut) \
	{ \
		return hkMemoryRouter::getInstance().pool()._bufRealloc<TYPE>( t, nold, reqInOut ); \
	}
HK_TEMPLATE_ALLOCATOR(Debug, debug)
HK_TEMPLATE_ALLOCATOR(Solver, solver)
HK_TEMPLATE_ALLOCATOR(Heap, heap)
HK_TEMPLATE_ALLOCATOR(Temp, temp)

#if defined(HK_DEBUG) && !defined(HK_PLATFORM_PS3_SPU)
extern void HK_CALL HK_ASSERT_OBJECT_SIZE_OK_FUNC(hk_size_t nbytes);
#	define HK_ASSERT_OBJECT_SIZE_OK(A) HK_ASSERT_OBJECT_SIZE_OK_FUNC(A)
#else
#	define HK_ASSERT_OBJECT_SIZE_OK(A)
#endif

#endif // HKBASE_hkMemoryRouter_H

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
