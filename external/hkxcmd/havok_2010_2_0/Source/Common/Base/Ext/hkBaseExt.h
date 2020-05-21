/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//HK_REFLECTION_PARSER_EXCLUDE_FILE

#ifndef HKBASE_HKBASEEXT_H
#define HKBASE_HKBASEEXT_H

// This file contains data types and definitions which mimic
// the internal havok ones, except that they all use an external allocator.
// Havok SDK code should never include this file - it is used by
// demo code, the hkg graphics library etc.

#include <Common/Base/hkBase.h>

class extAllocator 
{
	public:
			// Get the allocator. If one of the init methods has not
			// been called, this will call initDefault.
		static hkMemoryAllocator& HK_CALL getInstance();

			// Initialize default allocator, currently malloc.
		static void HK_CALL initDefault();
			// Initialize checking allocator, currently a leak detector.
		static void HK_CALL initChecking();
			// Quit, required by the checking allocator.
		static void HK_CALL quit();
};

struct extContainerAllocator
{
	hkMemoryAllocator& get(void*) { return extAllocator::getInstance(); }
};

template <typename TYPE>
HK_FORCE_INLINE TYPE* HK_CALL extAllocate(int numberOfObjects, int cl)
{
	return static_cast<TYPE*>( hkMemoryRouter::easyAlloc( extAllocator::getInstance(), numberOfObjects*hkSizeOfTypeOrVoid<TYPE>::val ));
}

template <typename TYPE>
HK_FORCE_INLINE void HK_CALL extDeallocate(TYPE* ptr)
{
	hkMemoryRouter::easyFree( extAllocator::getInstance(), static_cast<void *>(ptr));
}

template <typename TYPE>
HK_FORCE_INLINE TYPE* HK_CALL extAlignedAllocate(int align, int numberOfObjects, int cl)
{
	return static_cast<TYPE*>( hkMemoryRouter::alignedAlloc( extAllocator::getInstance(), numberOfObjects*hkSizeOfTypeOrVoid<TYPE>::val, align ));
}

template <typename TYPE>
HK_FORCE_INLINE void HK_CALL extAlignedDeallocate(TYPE* ptr)
{
	hkMemoryRouter::alignedFree( extAllocator::getInstance(), static_cast<void *>(ptr));
}

// make sure these are defined before we redefine them
#include <Common/Base/Container/String/hkStringBuf.h>
#include <Common/Base/Container/String/hkStringPtr.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

#	define EXT_OPERATOR_DELETE HK_FORCE_INLINE void HK_CALL operator delete(void*, void*) { }

#define EXT_DECLARE_REFOBJECT_ALLOCATOR() \
	HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t nbytes)	{ HK_ASSERT_OBJECT_SIZE_OK(nbytes); hkReferencedObject* b = static_cast<hkReferencedObject*>(extAllocator::getInstance().blockAlloc(static_cast<int>(nbytes))); b->m_memSizeAndFlags = static_cast<hkUint16>(nbytes); return b; } \
	HK_FORCE_INLINE void  HK_CALL operator delete(void* p)			{ hkReferencedObject* b = static_cast<hkReferencedObject*>(p); extAllocator::getInstance().blockFree(p, b->m_memSizeAndFlags); } \
	HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t, void* p)	{ return p; }	\
	EXT_OPERATOR_DELETE \
	HK_MUST_END_WITH_SEMICOLON

/* 
#define EXT_DECLARE_NONVIRTUAL_ALLOCATOR_BY_SIZE(CLASS_SIZE) \
	HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t nbytes)	{ HK_ASSERT(0x6c787b7f, nbytes == CLASS_SIZE ); void* obj = extAllocator::getInstance().blockAlloc(static_cast<int>(nbytes));  return obj; } \
	HK_FORCE_INLINE void  HK_CALL operator delete(void* p)			{ if (p) extAllocator::getInstance().blockFree(p, CLASS_SIZE); } \
	HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t n, void* p){ HK_ASSERT(0x77bb90a1, n == CLASS_SIZE); return p; } \
	EXT_OPERATOR_DELETE \
	HK_MUST_END_WITH_SEMICOLON 
	*/

#define EXT_DECLARE_NONVIRTUAL_ALLOCATOR_BY_TYPE(CLASS_TYPE) \
	HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t nbytes)	{ HK_ASSERT(0x6c787b7f, nbytes == sizeof(CLASS_TYPE) ); void* obj = extAllocator::getInstance().blockAlloc(int(sizeof(CLASS_TYPE))); return obj; } \
	HK_FORCE_INLINE void  HK_CALL operator delete(void* p)			{ if (p) { extAllocator::getInstance().blockFree(p, sizeof(CLASS_TYPE)); }  } \
	HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t n, void* p){ HK_ASSERT(0x77bb90a1, n == sizeof(CLASS_TYPE)); return p; } \
	EXT_OPERATOR_DELETE \
	HK_MUST_END_WITH_SEMICOLON 


#define EXT_DECLARE_NONVIRTUAL_ALLOCATOR(THIS_CLASS) \
	EXT_DECLARE_NONVIRTUAL_ALLOCATOR_BY_TYPE(THIS_CLASS)

#define hkContainerHeapAllocator extContainerAllocator
#define hkContainerTempAllocator extContainerAllocator
#define hkStringBuf extStringBuf
#define hkStringPtr extStringPtr

template <typename T>
class extArray : public hkArray<T,extContainerAllocator>
{
	public:

		typedef hkArray<T,extContainerAllocator> Type;
		typedef hkArray<T,extContainerAllocator> Temp;

		HK_FORCE_INLINE extArray() {}
		explicit HK_FORCE_INLINE extArray(int size) : hkArray<T, extContainerAllocator>(size) {}
		HK_FORCE_INLINE extArray(int size, const T& fill) : hkArray<T, extContainerAllocator>(size, fill) {}
		HK_FORCE_INLINE extArray(T* buffer, int size, int capacity) : hkArray<T, extContainerAllocator>(buffer, size, capacity) {}
};

template <typename T, int N, typename A=extContainerAllocator>
class extInplaceArray : public extArray<T>
{
	public:

		extInplaceArray() : extArray<T>(&m_storage[0], 0, N) {}
		extInplaceArray(const extInplaceArray& a) : extArray<T>(&m_storage[0], 0, N) { *this = a; }
		extInplaceArray(const extArray<T>& a) : extArray<T>(&m_storage[0], 0, N) { *this = a; }
		void operator=( const extInplaceArray& a ) { hkArray<T,A>::operator=(a); }
		void operator=( const extArray<T>& a ) { hkArray<T,A>::operator=(a); }

		T m_storage[N];
};

template <typename KEY, typename VAL=KEY, typename OPS=hkMapOperations<KEY>, typename Allocator=extContainerAllocator >
class extMap : public hkMap<KEY,VAL,OPS,Allocator>
{
};

#define hkInplaceArray extInplaceArray
#define hkInplaceArrayAligned16 extInplaceArrayAligned16
#define hkArrayBase extArrayBase
#define hkArray extArray
#define hkPointerMapStorage extPointerMapStorage
#define hkPointerMap extPointerMap
#define hkMap extMap

#undef HKBASE_HKSTRINGBUF_H
#include <Common/Base/Container/String/hkStringBuf.h>
#undef HKBASE_HKSTRINGPTR_H
#include <Common/Base/Container/String/hkStringPtr.h>
#undef HKBASE_POINTER_MAP_H
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

#undef hkContainerHeapAllocator
#undef hkInplaceArrayAligned16
#undef hkInplaceArray
#undef hkArrayBase
#undef hkArray
#undef hkStringBuf
#undef hkStringPtr
#undef hkPointerMapStorage
#undef hkPointerMap
#undef hkMap

class hkCriticalSection;

class extCriticalSection
{
	public:
		
		extCriticalSection(int spinCount = 0);
		~extCriticalSection();

		void enter();
		void leave();

		hkCriticalSection* get() { return reinterpret_cast<hkCriticalSection*>(&m_section[0]); }

	protected:

		HK_ALIGN16(hkUlong m_section[16]);
};

#endif // HKBASE_HKBASEEXT_H

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
