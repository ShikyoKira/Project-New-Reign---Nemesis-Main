/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template<typename T, typename OPS>
int hkMinHeap<T, OPS>::upHeap(int i)
{
	HK_ASSERT(0x2432a423, i >= 0 && i < m_contents.getSize());

	int currentIndex = i;
	int parentIndex = getParent(currentIndex);

	while (parentIndex >= 0 && !OPS::lessThan(m_contents[parentIndex], m_contents[currentIndex]))
	{
		// Swap
		OPS::swap(m_contents[currentIndex], m_contents[parentIndex]);
		// 
		currentIndex = parentIndex;
		parentIndex = getParent(currentIndex);
	}
	return currentIndex;
}

template<typename T, typename OPS>
void hkMinHeap<T, OPS>::addEntry(const T& entry)
{
	const int index = m_contents.getSize();
	OPS::setIndex(entry, index);
	m_contents.pushBack(entry);
	upHeap(index);
}

template <typename T, typename OPS>
int hkMinHeap<T, OPS>::_getSmallerChild(int i) const
{
	const int leftIndex = getLeftChild(i);
	const int rightIndex = getRightChild(i);
	const int heapSize = m_contents.getSize();

	if (rightIndex >= heapSize)
	{
		return leftIndex;
	}
	else
	{
		return OPS::lessThan(m_contents[leftIndex], m_contents[rightIndex]) ? leftIndex : rightIndex;
	}
}

template <typename T, typename OPS>
int hkMinHeap<T, OPS>::setEntry(int i, const T& newEntry)
{
	T& entry = m_contents[i];
	if (OPS::lessThan(newEntry, entry))
	{
		entry = newEntry;
		OPS::setIndex(entry, i);
		return upHeap(i);
	}
	if (OPS::lessThan(entry, newEntry))
	{
		entry = newEntry;
		OPS::setIndex(entry, i);
		return downHeap(i);
	}
	return i;
}

template <typename T, typename OPS>
int hkMinHeap<T, OPS>::downHeap(int i)
{
	HK_ASSERT(0x2432a423, i >= 0 && i < m_contents.getSize());
	
	int currentIndex = i;
	int heapSize = m_contents.getSize();

	// If rightIndex is bigger than heapSize, take the leftIndex. Otherwise, take the index with the smaller corresponding cost
	int swapIndex = _getSmallerChild(currentIndex);

	while (swapIndex < heapSize && OPS::lessThan(m_contents[swapIndex], m_contents[currentIndex]))
	{
		OPS::swap(m_contents[currentIndex], m_contents[swapIndex]);

		currentIndex = swapIndex;
		swapIndex = _getSmallerChild(currentIndex);
	}

	return currentIndex;
}

template <typename T, typename OPS>
void hkMinHeap<T, OPS>::removeEntry(int removeIndex)
{
	HK_ASSERT(0x2432a423, removeIndex >= 0 && removeIndex < m_contents.getSize());

	const int lastIndex = m_contents.getSize() - 1;
	if (removeIndex == lastIndex)
	{
		// Last thing, don't need to do anything special
		m_contents.popBack();
	}
	else
	{
		T* contents = m_contents.begin();
		// Must always be less
		const hkBool32 isLess = OPS::lessThan( contents[lastIndex], contents[removeIndex]);
		// Put the last entry at the remove index
		contents[removeIndex] = contents[lastIndex];
		OPS::setIndex(contents[removeIndex], removeIndex);
		m_contents.popBack();
		// Reshift back down again - must be down, as previously it was at the end of the heap
		if (isLess)
			upHeap(removeIndex);
		else
			downHeap(removeIndex);
	}
}

template <typename T, typename OPS>
hkBool32 hkMinHeap<T, OPS>::isOk() const
{
	// Verify that the heap matches up with the pairs
	const int size = m_contents.getSize();
	const T*const contents = m_contents.begin();

	for (int i = 0; i < size; i++)
	{
		const T& entry = contents[i];

		// Indexing is wrong
		if (!OPS::hasIndex(entry, i))
		{
			return false;
		}

		// Verify the heap property
		const int leftIndex = getLeftChild(i);
		const int rightIndex = getRightChild(i);

		if (leftIndex < size)
		{
			if (OPS::lessThan(contents[leftIndex], entry))
			{
				return false;
			}
		}
		if (rightIndex < size)
		{
			if (OPS::lessThan(contents[rightIndex], entry))
			{
				return false;
			}
		}
	}

	return true;
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
