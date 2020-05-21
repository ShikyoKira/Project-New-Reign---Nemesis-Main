/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HKALGORITHM_H
#define HKBASE_HKALGORITHM_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Algorithm/Sort/hkRadixSort.h>

namespace hkAlgorithm
{

	/// swap the elements
	template<typename T>
	HK_FORCE_INLINE void HK_CALL swap(T& x, T& y)
	{
		T t(x);
		x = y;
		y = t;
	}


	/// swap the 16 byte aligned elements
	template<typename T>
	HK_FORCE_INLINE void HK_CALL swap16(T& x, T& y)
	{
		HK_ALIGN16( T t ) = x;
		x = y;
		y = t;
	}
			
	/// use to change endian-ness. Swaps the bytes pointed to by start.
	HK_FORCE_INLINE void swapBytes(void* start, int count)
	{
		hkUint8* front = (hkUint8*)start;
		hkUint8* back  = (hkUint8*)hkAddByteOffset(start, count-1);
		for (int i = 0; i < count / 2; i++)
		{
			hkAlgorithm::swap(*(front++), *(back--));
		}
	}

	/// function object that routes calls to operator<
	template<typename T>
	class less
	{
		public:

			HK_FORCE_INLINE hkBool32 operator() ( const T& a, const T& b )
			{
				return ( a < b );
			}
	};


	/// function object that routes calls to operator>
	template<typename T>
	class greater
	{
	public:

		HK_FORCE_INLINE hkBool32 operator() ( const T& a, const T& b )
		{
			return ( a > b );
		}
	};


	/// heap sort. You supply the functor, see hkAlgorithm::less for an example functor.
	template<typename T, typename L>
	void HK_CALL heapSort(T *pArr, int iSize, L cmpLess);


	/// Heap sort for the elements of the specified array.
	///
	/// \param *pArr		A pointer to the array to sort.
	/// \param iSize		The size of the array pointed to by *pArr.
	///
	template<typename T>
	void HK_CALL heapSort(T *pArr, int iSize)
	{
		heapSort( pArr, iSize, less<T>() );
	}

	// used by heapSort
	template<typename T, typename L>
	void HK_CALL downHeap(T *pArr, int k, int n, L cmpLess);

	template<typename T, typename L>
	void HK_CALL insertionSort(T* pArr, int size, L cmpLess);

	template<typename T>
	void HK_CALL insertionSort(T* pArr, int size)
	{
		insertionSort( pArr, size, less<T>() );
	}

	/// quick sort. You supply the functor, see hkAlgorithm::less for an example functor.
	template<typename T, typename L>
	HK_FORCE_INLINE	void HK_CALL quickSort(T *pArr, int iSize, L cmpLess);


	/// Quick sort for the elements of the specified array.
	///
	/// \param *pArr		A pointer to the data to sort.
	/// \param iSize		The size of the array pointed to by *pArr.
	///
	template<typename T>
	HK_FORCE_INLINE	void HK_CALL quickSort(T *pArr, int iSize)
	{
		quickSort( pArr, iSize, less<T>() );
	}

	template< typename T, typename L >
	void HK_CALL quickSortRecursive(T *pArr, int d, int h, L cmpLess);

        /// Quick sort using an explicit stack for efficiency reasons.
	template<typename T>
    HK_FORCE_INLINE void HK_CALL explicitStackQuickSort(T *pArr, int iSize)
	{
        explicitStackQuickSort( pArr, iSize, less<T>() );
	}


	/// Bubble sort. You supply the functor, see hkAlgorithm::less for an example functor.
	template<typename T, typename L>
	HK_FORCE_INLINE	void HK_CALL bubbleSort(T *pArr, int iSize, L cmpLess);


	/// Bubble sort for the elements of the specified array.
	/// THIS IS NOT AN EFFICIENT SORTING ALGORITHM: O(n^2).
	/// It's almost always better to use quickSort or heapSort.
	/// Unlike quickSort or heapSort, this algorithm is STABLE:
	/// the relative order of equal elements is preserved.
	/// Bubble sort is a good choice for sorting or re-sorting a list
	/// that is approximately sorted already.
	///
	/// \param *pArr		A pointer to the data to sort.
	/// \param iSize		The size of the array pointed to by *pArr.
	///
	template<typename T>
	HK_FORCE_INLINE	void HK_CALL bubbleSort(T *pArr, int iSize)
	{
		bubbleSort( pArr, iSize, less<T>() );
	}

	struct ListElement
	{
		ListElement* next;
	};
		/// Inplace singly linked list sort.
		/// Elements are sorted by address.
		/// Performs a merge sort (O(n log n) without any allocations.
	ListElement* _sortList(ListElement* head);

		/// Inplace singly linked list sort.
		/// Assumes the first element of each object is a pointer to next.
		/// Elements are sorted by address.
		/// Performs a merge sort (O(n log n) without any allocations.
	template<typename T>
	T* sortList(T* head)
	{
		return reinterpret_cast<T*>( _sortList( reinterpret_cast<ListElement*>(head) ) );
	}
}

#define hkSort hkAlgorithm::quickSort
//#define hkSort hkAlgorithm::explicitStackQuickSort

#include <Common/Base/Algorithm/Sort/hkSort.inl>

#endif // HKBASE_HKALGORITHM_H

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
