/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


template <typename T>
HK_FORCE_INLINE hkSmallArray<T>::hkSmallArray()
	:	m_data(HK_NULL),
		m_size(0),
		m_capacityAndFlags(DONT_DEALLOCATE_FLAG)
{
}


template <typename T>
HK_FORCE_INLINE hkSmallArray<T>::hkSmallArray(int n)
	:	m_data(hkAllocateChunk<T>(n, HK_MEMORY_CLASS_ARRAY)),
		m_size(hkUint16(n)),
		m_capacityAndFlags(hkUint16(n))
{
	if (!n)
	{
		m_capacityAndFlags |= DONT_DEALLOCATE_FLAG;
	}
}


template <typename T>
HK_FORCE_INLINE hkSmallArray<T>::~hkSmallArray()
{
	releaseMemory();
}


template <typename T>
HK_FORCE_INLINE void hkSmallArray<T>::clearAndDeallocate()
{
	releaseMemory();
	m_data = HK_NULL;
	m_size = 0;
	m_capacityAndFlags = hkUint16(DONT_DEALLOCATE_FLAG | (m_capacityAndFlags & LOCKED_FLAG));
}


template <typename T>
HK_FORCE_INLINE T& hkSmallArray<T>::operator[] (int i)
{
	HK_ASSERT(0x394e9c6c,  i >= 0 && i < m_size );
	return m_data[i];
}


template <typename T>
HK_FORCE_INLINE const T& hkSmallArray<T>::operator[] (int i) const
{
	HK_ASSERT(0x264718f3,  i >= 0 && i < m_size  );
	return m_data[i];
}


template <typename T>
HK_FORCE_INLINE int hkSmallArray<T>::getSize() const
{
	return m_size;
}


template <typename T>
HK_FORCE_INLINE int hkSmallArray<T>::getCapacity() const
{
	return (m_capacityAndFlags & static_cast<int>(CAPACITY_MASK));
}


template <typename T>
HK_FORCE_INLINE T& hkSmallArray<T>::back()
{
	HK_ASSERT(0x52595f9b, m_size );
	return m_data[ m_size - 1 ];
}


template <typename T>
HK_FORCE_INLINE const T& hkSmallArray<T>::back() const
{
	HK_ASSERT(0x6e984e36, m_size );
	return m_data[ m_size -1 ];
}


template <typename T>
HK_FORCE_INLINE hkBool hkSmallArray<T>::isEmpty() const
{
	return m_size == 0;
}


template <typename T>
HK_FORCE_INLINE void hkSmallArray<T>::releaseMemory()
{
	if( (m_capacityAndFlags & DONT_DEALLOCATE_FLAG) == 0)
	{
		hkDeallocateChunk( m_data, getCapacity(), HK_MEMORY_CLASS_ARRAY);
	}
}


template <typename T>
HK_FORCE_INLINE void hkSmallArray<T>::removeAt(int i)
{
	HK_ASSERT(0x63bab20a,  i >= 0 && i < m_size);
	m_size--;
	m_data[i] = m_data[m_size];
}


template <typename T>
HK_FORCE_INLINE void hkSmallArray<T>::removeAtAndCopy(int index)
{
	HK_ASSERT(0x453a6437,  index >= 0 && index < m_size);
	m_size--;
	for(int i = index; i < m_size; ++i)
	{
		m_data[i] = m_data[i+1];
	}
}


template <typename T>
HK_FORCE_INLINE int hkSmallArray<T>::indexOf(const T& t) const
{
	for(int i = 0; i < m_size; ++i)
	{
		if( m_data[i] == t )
		{
			return i;
		}
	}
	return -1;
}


template <typename T>
HK_FORCE_INLINE T& hkSmallArray<T>::expandOne( )
{
	if(m_size == getCapacity())
	{
#ifdef HK_DEBUG
		if ( isLocked() )
			HK_WARN(0xf3768206, "hkSmallArray::expandOne on locked array. Destructor (memory dealloc) will not be called.");
#endif	
		hkSmallArrayUtil::_reserveMore( this, sizeof(T) );
	}
	return m_data[m_size++];
}


template <typename T>
HK_FORCE_INLINE void hkSmallArray<T>::pushBack(const T& t)
{
	if(m_size == getCapacity())
	{
#ifdef HK_DEBUG
		if ( isLocked() )
			HK_WARN(0xf3768206, "hkArray::pushBack on locked array. Destructor (memory dealloc) will not be called.");
#endif
		hkSmallArrayUtil::_reserveMore( this, sizeof(T) );
	}
	m_data[m_size++] = t;
}


template <typename T>
typename hkSmallArray<T>::iterator hkSmallArray<T>::begin() 
{
	return m_data;
}


template <typename T>
typename hkSmallArray<T>::iterator hkSmallArray<T>::end()
{
	return m_data + m_size;
}


template <typename T>
typename hkSmallArray<T>::const_iterator hkSmallArray<T>::begin() const
{
	return m_data;
}


template <typename T>
typename hkSmallArray<T>::const_iterator hkSmallArray<T>::end() const
{
	return m_data + m_size;
}


template <typename T>
hkBool hkSmallArray<T>::isLocked()
{
	return (m_capacityAndFlags & LOCKED_FLAG) != 0;
}


template <typename T>
HK_FORCE_INLINE void HK_CALL hkSmallArray<T>::copyBackwards(T* dst, const T* src, int n)
{
	for(int i = n-1; i >= 0; --i)
	{
		dst[i] = src[i];
	}
}


template <typename T>
HK_FORCE_INLINE void hkSmallArray<T>::reserve(int n)
{
	const int capacity = getCapacity();
	if( capacity < n)
	{
#ifdef HK_DEBUG
		if ( isLocked() )
			HK_WARN(0xf3768206, "hkArray::reserve on locked array. Destructor (memory dealloc) will not be called.");
#endif
		int cap2 = 2 * capacity;
		int newSize = (n < cap2) ? cap2 : n;
		hkSmallArrayUtil::_reserve(this, newSize, sizeof(T));
	}
}


template <typename T>
void hkSmallArray<T>::insertAt(int index, const T* p, int numtoinsert)
{
	HK_ASSERT(0x4cbc67c5,  index >= 0 && index <= getSize() );
	const int newsize     = numtoinsert + m_size;
	const int numtomove   = m_size - index;
	if(newsize > getCapacity())
	{
		// note double copy from [i:end] not a problem in practice
		reserve(newsize); // will warn if locked.
	}
	hkMemUtil::memCpyBackwards(m_data + index + numtoinsert, m_data + index, numtomove*sizeof(T));
	hkMemUtil::memCpyBackwards(m_data + index, p, numtoinsert*sizeof(T));
	m_size = hkUint16(newsize);
}


template <typename T>
void hkSmallArray<T>::insertAt(int i, const T& t)
{
	insertAt(i, &t, 1 );
}


template <typename T>
HK_FORCE_INLINE void hkSmallArray<T>::popBack( int numRemove )
{
	HK_ASSERT(0x5b57310e, m_size >= hkUint16(numRemove) );
	m_size = hkUint16(m_size - numRemove);
}


template <typename T>
HK_FORCE_INLINE void hkSmallArray<T>::removeAllAndCopy(const T& t)
{
	int destination = 0;
	// Do no copying until we've found a t.
	while ( ( destination < m_size ) && ( m_data[destination] != t ) )
	{
		++destination;
	}
	// If we have found a t, start copying.
	for ( int source = destination; destination < m_size; ++source )
	{
		if ( m_data[source] != t )
		{
			m_data[destination] = m_data[source];
			++destination;
		}
		else
		{
			--m_size;
		}
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
