/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SORT_INL
#define HK_SORT_INL

// Heap Sort, O(n log(n)), not recursive
// see http://www.csse.monash.edu.au/~lloyd/tildeAlgDS/Sort/Heap.html for explanation

namespace hkAlgorithm
{

template<typename T, typename L>
void HK_CALL downHeap(T *pArr, int k, int n, L cmpLess)
{
	// precondition: array pArr[k+1,n] is a heap
	// postcondition: array pArr[k,n] is a heap

	T temp = pArr[k - 1];
	//find them maximum of the new 'parent' and all of its children
	while (k <= n/2)
	{
		int child = 2*k;
		//don't process children 'outside' of the array
		if ((child < n) && cmpLess(pArr[child - 1], pArr[child]))
		{
			child++;
		}
		//take the maximum
		if (cmpLess(temp, pArr[child - 1]))
		{
			pArr[k - 1] = pArr[child - 1];
			k = child;
		}
		else
		{
			break;
		}
	}
	pArr[k - 1] = temp;
}


template<typename T, typename L>
void HK_CALL heapSort(T *pArr, int iSize, L cmpLess)
{
	int k;
	int n = iSize;
	//create a heap
	for (k = n/2; k > 0; k--)
	{
	    downHeap(pArr, k, n, cmpLess);
    }

	// now swap the
	while ( n>=1 )
	{
		//a[0] is now known to be largest, so put it to the end, and downheap 0..n-1
		T temp = pArr[0];
		pArr[0] = pArr[n - 1];
		pArr[n - 1] = temp;

		n = n - 1;
		downHeap(pArr, 1, n, cmpLess);
	}
}

template<typename T, typename L>
HK_FORCE_INLINE
void HK_CALL insertionSort(T* pArr, int size, L cmpLess)
{
	for(int i=0; i<size; i++)
	{
		T key = pArr[i];
		int j = i;
		while( j > 0 && cmpLess(key, pArr[j-1]) )
		{
			pArr[j] = pArr[j-1];
			j--;
		}
		pArr[j] = key;
	}
}

template<typename T, typename L>
void HK_CALL quickSortRecursive(T *pArr, int d, int h, L cmpLess)
{
	int i,j;
	HK_ALIGN16( T str );
begin:
	i = h;
	j = d;

	str = pArr[(d+h)>>1];

	do {

		while ( cmpLess( pArr[j], str) ){ j++; }
		while ( cmpLess( str, pArr[i]) ){ i--; }

		if ( i >= j )
		{
			if ( i != j )
			{
				HK_ALIGN16( T zal );
				T& pi = pArr[i];
				T& pj = pArr[j];
				zal = pi; pi = pj, pj = zal;
			}

			i--;
			j++;
		}
	} while (j <= i);

	if (d < i)
	{
		quickSortRecursive(pArr,d,i, cmpLess);
	}

	if (j < h)
	{
		d = j;
		goto begin;
	}
}

template<typename T, typename L>
void HK_CALL quickSort(T *pArr, int iSize, L cmpLess)
{
	if (iSize > 1)
	{
		int	low = 0;
		int high = iSize - 1;
		quickSortRecursive(pArr,low,high, cmpLess);
	}
}


template<typename T, typename L>
void HK_CALL explicitStackQuickSort(T* base, int numMem, L cmpLess)
{
    HK_ALIGN16( T swap );
    HK_ALIGN16( T pivot );
    const int maxDepth = sizeof(base)*8;

	if (numMem <=1 ) return;

    T* lbStack[maxDepth],* ubStack[maxDepth];

    lbStack[0] = base;
    ubStack[0] = base+(numMem-1);

    for (int stackPos = 0; stackPos >= 0; stackPos--)
    {
        T* lb = lbStack[stackPos];
        T* ub = ubStack[stackPos];
        while (true)
        {
            T* j = lb;
            T* i = ub;

            pivot = lb[(ub-lb)>>1];
            do
            {
                while ( cmpLess( *j, pivot) ){ j++; }
                while ( cmpLess( pivot, *i) ){ i--; }

                if ( i >= j )
                {
                    if ( i != j )
                    {
                        swap = *i;
                        *i = *j;
                        *j = swap;
                    }
                    i--;
                    j++;
                }
            } while (j <= i);

            if (lb < i)
            {
                if (j < ub)
                {
                        // Both are sortable
                        // Stack the smaller of the two sub-parts
                    if (ub - j > i-lb)
                    {
                        lbStack[stackPos] = j;
                        ubStack[stackPos++] = ub;
                        //
                        ub = i;
                    }
                    else
                    {
                        // Stack it
                        lbStack[stackPos] = lb;
                        ubStack[stackPos++] = i;
                        //
                        lb = j;
                    }
                    continue;
                }
                else
                {
                    ub = i;
                    continue;
                }
            }
            else
            {
                if (j < ub)
                {
                    lb = j;
                    continue;
                }
            }
            break;
        }
    }
}

template< typename T, typename L >
void HK_CALL bubbleSort( T *pArr, int iSize, L cmpLess )
{
	bool didSwap = true;

	for ( int i = 1; (i < iSize) && didSwap; i++ )
	{
		didSwap = false;

		for ( int j = 0; j < iSize - i; j++ )
		{
			if ( cmpLess( pArr[ j + 1 ], pArr[ j ] ) )
			{
				swap( pArr[ j + 1 ], pArr[ j ] );

				didSwap = true;
			}
		}
	}
}

}  // namespace hkAlgorithm

#endif // HK_SORT_INL

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
