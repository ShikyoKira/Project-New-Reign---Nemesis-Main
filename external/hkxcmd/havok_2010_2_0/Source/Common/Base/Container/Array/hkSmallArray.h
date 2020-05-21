/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HKSMALLARRAY_H
#define HKBASE_HKSMALLARRAY_H

#include <Common/Base/Memory/Util/hkMemUtil.h>

#ifndef hkArrayAllocator
#	define hkArrayAllocator hkThreadMemory::getInstance()
#endif


/// Common functionality for all hkSmallArray types.
/// These are out of line functions to avoid code bloat.
namespace hkSmallArrayUtil
{
	void HK_CALL _reserve(void*, int numElem, int sizeElem);
	void HK_CALL _reserveMore(void* array, int sizeElem);
}


/// A 14bit array.
/// Note that, for performance reasons, order may not be preserved when deleting elements.
template <typename T>
class hkSmallArray
{
		friend void HK_CALL hkSmallArrayUtil::_reserve(void*, int numElem, int sizeElem);
		friend void HK_CALL hkSmallArrayUtil::_reserveMore(void* array, int sizeElem);

	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ARRAY, hkSmallArray<T>);

			/// Creates a zero length array.
		HK_FORCE_INLINE hkSmallArray();

			/// Creates an array of size n. All elements are uninitialized.
		explicit HK_FORCE_INLINE hkSmallArray(int size);

			/// Deallocates array memory.
		HK_FORCE_INLINE ~hkSmallArray();



			/// Returns the size.
		HK_FORCE_INLINE int getSize() const;

			/// Returns the capacity.
		HK_FORCE_INLINE int getCapacity() const;

			/// Checks if the size is zero.
		HK_FORCE_INLINE hkBool isEmpty() const;

			/// Checks if the locked flag is set.
			/// If an array is locked, the storage has come from file
			/// (so the do not deallocate flag is set) but it also
			/// means that the destructor will never be called so if you resize
			/// the array you will have to make sure and call clearAndDeallocate()
			/// yourself.
		HK_FORCE_INLINE hkBool isLocked();



			/// Increments the size by 1 and returns a reference to the first element created.
		HK_FORCE_INLINE T& expandOne( );

			/// Inserts the array a at index i.
			/// See also getSubarray() and the constructor, which uses an existing
			/// C style array in place.
		void insertAt(int i, const T* p, int numElems );

			/// Inserts t at index i.
			/// Elements from i to the end are copied up one place.
		void insertAt(int i, const T& t);

			/// Removes the element at the specified index. The last array element is used to replace the removed element, and the size is reduced by 1.
			/// This is very fast, but note that the order of elements is changed.
		void removeAt(int index);

			/// Removes the element at the specified index, copying elements down one slot as in the STL array.
			/// Slower than removeAt(), but the order is unchanged.
		void removeAtAndCopy(int index);



			/// Read/write access to the i'th element.
		HK_FORCE_INLINE T& operator[] (int i);

			/// Read only access to the i'th element.
		HK_FORCE_INLINE const T& operator[] (int i) const;

			/// Returns the index of the first occurrence of t, or -1 if not found.
		int indexOf(const T& t) const;

			/// Removes the last element.
		HK_FORCE_INLINE void popBack( int numElemsToRemove = 1 );

			/// Adds an element to the end.
		HK_FORCE_INLINE void pushBack(const T& e);

			/// Read/write access to the last element.
		HK_FORCE_INLINE T& back();

			/// Read only access to the last element.
		HK_FORCE_INLINE const T& back() const;

			/// Removes all occurrences of t, preserving the order of the remaining elements.
		HK_FORCE_INLINE void removeAllAndCopy(const T& t);

			/// 
		typedef T* iterator;
			/// 
		typedef const T* const_iterator;

			/// Returns an STL-like iterator to the first element.
		HK_FORCE_INLINE iterator begin();
			/// Returns an STL-like iterator to the 'one past the last' element.
		HK_FORCE_INLINE iterator end();
			/// Returns an STL-like const iterator to the first element.
		HK_FORCE_INLINE const_iterator begin() const;
			/// Returns an STL-like const iterator to the 'one past the last' element.
		HK_FORCE_INLINE const_iterator end() const;



			/// Ensures no reallocation occurs until at least size n.
		HK_FORCE_INLINE void reserve(int n);

		static void HK_CALL copyBackwards(T* dst, const T* src, int n);

			/// Sets the size to zero and deallocates storage.
		void clearAndDeallocate();


	public:

		// Public so that the serialization can access it.
		enum
		{
			CAPACITY_MASK			= hkUint16(0x3fff),
			FLAG_MASK				= hkUint16(0xC000),
			DONT_DEALLOCATE_FLAG	= hkUint16(0x8000), // Indicates that the storage is not the array's to delete
			LOCKED_FLAG				= hkUint16(0x4000), // Indicates that the array will never have its dtor called (read in from packfile for instance)
			FORCE_SIGNED			= -1
		};

	protected:

		friend class hkStatisticsCollector;
		hkSmallArray(const hkSmallArray&) { }
		void operator=(const hkSmallArray&) { }

		void releaseMemory();

		T*			m_data;
		hkUint16	m_size; 
		hkUint16	m_capacityAndFlags; // highest 2 bits indicate any special considerations about the allocation for the array

	public:

			/// For serialization, we want to initialize the vtables
			/// in classes post data load, and NOT call the default constructor
			/// for the arrays (as the data has already been set).
		hkSmallArray(hkFinishLoadedObjectFlag f) { }

};


#include <Common/Base/Container/Array/hkSmallArray.inl>


#undef hkArrayAllocator


#endif // HKBASE_HKSMALLARRAY_H

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
