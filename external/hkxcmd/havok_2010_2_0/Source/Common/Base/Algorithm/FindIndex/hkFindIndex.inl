/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

namespace hkAlgorithm
{
	template< typename T, typename V, typename EvalFunc, typename CompareLess>
	inline _FindMinimumImpl<T, V, EvalFunc, CompareLess>::_FindMinimumImpl( const T* pArray, int n, const EvalFunc& evalFunc, CompareLess less )
		:	m_bestIndex(0)
	{
		HK_ASSERT(0x25e91497, n >= 1);
		HK_ASSERT(0x4b51b364, pArray != HK_NULL );
		
		const T* p  = pArray;
		
		m_bestValue = (V) evalFunc( *p++ );
		for( int i = 1; i < n; ++i )
		{
			V t = (V) evalFunc( *p++ );
			
			hkBool32 isLess = less(t,m_bestValue);
			m_bestValue = (isLess ? t : m_bestValue);
			m_bestIndex = (isLess ? i : m_bestIndex);
			}
		}

	template< typename T, typename V, typename EvalFunc, typename CompFunc >
	int HK_CALL findMinimumIndexAndValue(const T *pArr, int N, V& valOut, const EvalFunc& evalFunc, CompFunc compFunc )
	{
		_FindMinimumImpl<T, V, EvalFunc, CompFunc > findMin(pArr, N, evalFunc, compFunc );
		valOut = findMin.m_bestValue;
		return findMin.m_bestIndex;
	}

	template< typename T, typename V, typename EvalFunc >
	int HK_CALL findMinimumIndexAndValue(const T *pArr, int N, V& valOut, const EvalFunc& evalFunc)
	{
		return findMinimumIndexAndValue( pArr, N, valOut, evalFunc, hkAlgorithm::less<V>() );
	}

	template< typename T, typename V, typename EvalFunc >
	int HK_CALL findMaximumIndexAndValue(const T *pArr, int N, V& valOut, const EvalFunc& evalFunc)
	{
		return findMinimumIndexAndValue( pArr, N, valOut, evalFunc, hkAlgorithm::greater<V>() );
	}



	template< typename T, typename EvalFunc, typename CompFunc >
	int HK_CALL findMinimumIndex(const T *pArr, int N, const EvalFunc& evalFunc, CompFunc compFunc )
	{
		_FindMinimumImpl<T, hkReal, EvalFunc, CompFunc > findMin(pArr, N, evalFunc, compFunc );
		return findMin.m_bestIndex;
	}

	template< typename T, typename EvalFunc >
	int HK_CALL findMinimumIndex(const T *pArr, int N, const EvalFunc& evalFunc )
	{
		return findMinimumIndex(pArr, N, evalFunc, hkAlgorithm::less<hkReal>() );
	}

	template< typename T, typename EvalFunc >
	int HK_CALL findMaximumIndex(const T *pArr, int N, const EvalFunc& evalFunc )
	{
		return findMinimumIndex(pArr, N, evalFunc, hkAlgorithm::greater<hkReal>() );
	}

}

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
