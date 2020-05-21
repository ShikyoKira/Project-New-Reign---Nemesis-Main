/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIND_INDEX
#define HK_FIND_INDEX

#include <Common/Base/Algorithm/Sort/hkSort.h>

namespace hkAlgorithm
{
	// A few simple functions to help with finding the "best" element of an array
	// The evaluation and comparison functions are fully general, but some common implementations are provided as well

	//
	// Finding the index of the minimum/maximum
	//

		/// Returns the index that gives the minimum value for evalFunc
	template< typename T, typename EvalFunc, typename CompFunc >
	int HK_CALL findMinimumIndex(const T *pArr, int N, const EvalFunc& evalFunc, CompFunc compFunc );

		/// Returns the index that gives the maximum value for evalFunc
	template< typename T, typename EvalFunc >
	int HK_CALL findMinimumIndex(const T *pArr, int N, const EvalFunc& evalFunc );

		/// Returns the index that gives the minimum value (using compFunc for comparsion) for evalFunc
	template< typename T, typename EvalFunc >
	int HK_CALL findMaximumIndex(const T *pArr, int N, const EvalFunc& evalFunc );


	//
	// Finding the index and value of the minimum/maximum
	//

		/// Returns the index that gives the minimum value for evalFunc, and saves the value in valOut
	template< typename T, typename V, typename EvalFunc >
	int HK_CALL findMinimumIndexAndValue(const T *pArr, int N, V& valOut, const EvalFunc& evalFunc);

		/// Returns the index that gives the maximum value for evalFunc, and saves the value in valOut
	template< typename T, typename V, typename EvalFunc >
	int HK_CALL findMaximumIndexAndValue(const T *pArr, int N, V& valOut, const EvalFunc& evalFunc);

		/// Returns the index that gives the minimum value (using compFunc for comparison) for evalFunc, and saves the value in valOut
	template< typename T, typename V, typename EvalFunc, typename CompFunc >
	int HK_CALL findMinimumIndexAndValue(const T *pArr, int N, V& valOut, const EvalFunc& evalFunc, CompFunc compFunc );

	//
	// Helper struct
	//
	template< typename T, typename V, typename EvalFunc, typename CompareLess>
	struct _FindMinimumImpl
	{
	public:
		HK_FORCE_INLINE _FindMinimumImpl( const T* a, int n, const EvalFunc& evalFunc, CompareLess less);

		int m_bestIndex;
		V m_bestValue;
	};

}

#include <Common/Base/Algorithm/FindIndex/hkFindIndex.inl>
#endif // HK_FIND_INDEX

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
