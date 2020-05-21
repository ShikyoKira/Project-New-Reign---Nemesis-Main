/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template <typename T>
hkQueue<T>::hkQueue()
	:	m_data( HK_NULL ),
		m_capacity( 0 ),
		m_head(0),
		m_tail(0),
		m_elementsInUse(0)
{
}


template <typename T>
hkQueue<T>::hkQueue( int n  )
	:	m_data(hkAllocateChunk<T>( n , HK_MEMORY_CLASS_ARRAY )),
		m_capacity(n),
		m_head(0),
		m_tail(0),
		m_elementsInUse(0)
{
	HK_ASSERT2( 0x4091ba45, n > 0, "this constructor is not appropriate for zero initial capacity" );
}


template <typename T>
hkQueue<T>::~hkQueue()
{
	releaseMemory();
}



template <typename T>
inline hkBool hkQueue<T>::isEmpty() const
{
	return m_elementsInUse == 0;
}


template <typename T>
inline int hkQueue<T>::getSize() const
{
	return m_elementsInUse;
}


template <typename T>
inline int hkQueue<T>::getCapacity() const
{
	return m_capacity;
}


template <typename T>
inline void hkQueue<T>::clear()
{
	m_head = 0;
	m_tail = 0;
	m_elementsInUse = 0;
}


template <typename T>
inline void hkQueue<T>::releaseMemory()
{
	if( m_capacity )
	{
		hkDeallocateChunk<T>( m_data, m_capacity, HK_MEMORY_CLASS_ARRAY );
	}
}


template <typename T>
inline void hkQueue<T>::setCapacity( int n )
{
	if( m_capacity < n)
	{
		// allocate a new buffer and copy existing data over
		if ( m_capacity * 2 >= n)
		{
			n = m_capacity * 2;
		}

		T* p = hkAllocateChunk<T>( n , HK_MEMORY_CLASS_ARRAY );

		if( m_data != HK_NULL )
		{
			if ( m_elementsInUse)
			{
				if ( m_tail <= m_head )
				{
					// split
					int numHead = m_capacity - m_head;
					int numTail = m_tail;
					hkString::memCpy( p, m_data+m_head, ( sizeof(T) * numHead ) );
					hkString::memCpy( p + numHead, m_data, ( sizeof(T) * numTail ) );
				}
				else
				{
					// one block
					hkString::memCpy( p, m_data+m_head, ( sizeof(T) * m_elementsInUse ) );
				}
			}
			m_head = 0;
			m_tail = m_elementsInUse;
		}

		// release the old buffer
		releaseMemory();

		// assign to new buffer and set capacity
		m_data = (T*)p;
		m_capacity = n;
	}
}

template <typename T>
inline void hkQueue<T>::increaseCapacity( )
{
	if( m_capacity == 0 )
	{
		// if there is no current capacity default to 8 elements
		setCapacity( 8 );
	}
	else
	{
		setCapacity( m_capacity * 2 );
	}
}


// Places a new element to the back of the queue and expand storage if necessary.
template <typename T>
inline void hkQueue<T>::enqueue( const T& element )
{
    if( m_elementsInUse >= m_capacity )
    {
	    increaseCapacity();
    }

	if( m_tail == m_capacity )
	{
		m_tail = 0;
	}
	
	m_data[m_tail++] = element;

	m_elementsInUse++;

}

// Places a new element to the front of the queue and expand storage if necessary.
template <typename T>
inline void hkQueue<T>::enqueueInFront( const T& element )
{
    if( m_elementsInUse >= m_capacity )
    {
	    increaseCapacity();
    }

    if( m_head == 0 )
    {
	    m_head = m_capacity;
    }

    m_data[--m_head] = element;
    m_elementsInUse++;
}


template <typename T>
inline void hkQueue<T>::dequeue( T& data )
{
    HK_ASSERT(0xf032ed23, m_elementsInUse );
    {
	    data = m_data[m_head];
	    if( ++m_head == m_capacity )
	    {
		    m_head = 0;
	    }
	    m_elementsInUse--;
    }
}

template <typename T>
inline void hkQueue<T>::peek( T& data ) const
{
	data = m_data[m_head];
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
