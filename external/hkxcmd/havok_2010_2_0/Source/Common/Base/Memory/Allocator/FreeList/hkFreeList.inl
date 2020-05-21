/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE void hkFreeList::allocBatch(void** out,int sizeIn)
{
	void** end = out + sizeIn;
	void** cur = out;

	Element* head = m_free;
	while (cur < end && head)
	{
		*cur++ = reinterpret_cast<void*>(head);
		head = head->m_next;
	}
	m_free = head;
	// Account for number of blocks allocated
	m_numFreeElements -= (cur - out);

	out = cur;
	// If still remaining we need more space
	while (cur < end)
	{
		if (m_top < m_blockEnd)
		{
			while (m_top < m_blockEnd && cur < end)
			{
				*cur++ = m_top;
				m_top += m_elementSize;
			}
			// Fix the number of elements
			m_numFreeElements -= (cur - out);
			// We need to update based on current pos
			out = cur;
		}
		else
		{
			// This will do an alloc + update m_numFreeElements
			*cur++ = addSpace();
			// Its accounted for so update out
			out = cur;
		}
	}
}

HK_FORCE_INLINE void hkFreeList::freeBatch(void** in,int sizeIn)
{
	m_numFreeElements += sizeIn;
	Element* head = m_free;
	while (--sizeIn >= 0)
	{
		Element* ele = reinterpret_cast<Element *>(*in++);
		if (ele)
		{
			ele->m_next = head;
			head = ele;
		}
		else
		{
			m_numFreeElements--;
		}
	}
	m_free = head;
}

HK_FORCE_INLINE void* hkFreeList::alloc()
{
	// if there is a block on the freelist, then just return that
	if (m_free)
	{
		m_numFreeElements --;
		// Grab the block
		void* data = reinterpret_cast<void*>(m_free);
		// Make m_free point to the next free block
		m_free = m_free->m_next;
		// Return the block
		return data;
	}

	// If there is no space after top, add some space
	if (m_top >= m_blockEnd)
	{
		return addSpace();
	}

	m_numFreeElements--;

	// Else take the memory from the top of the current block
	void* data = reinterpret_cast<void*>(m_top);
	m_top += m_elementSize;
	return data;
}

HK_FORCE_INLINE hkBool hkFreeList::canAlloc()
{
	if (m_free || m_top >= m_blockEnd)
	{
		return true;
	}
	void* ptr = addSpace();
	if (ptr == HK_NULL)
	{
		return false;
	}
	free(ptr);
	return true;
}

HK_FORCE_INLINE void hkFreeList::free(void* data)
{
	// We've got a free element
	m_numFreeElements++;
	// Its an element now - add it to the list
	Element* ele = reinterpret_cast<Element *>(data);
	ele->m_next = m_free;
	m_free = ele;
}

inline void hkFreeList::garbageCollect()
{
	findGarbage();
	freeAllFreeBlocks();
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
