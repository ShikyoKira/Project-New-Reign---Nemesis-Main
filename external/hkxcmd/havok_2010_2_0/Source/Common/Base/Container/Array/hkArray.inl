/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



template <typename T>
HK_FORCE_INLINE T& hkArrayBase<T>::operator[] (int i)
{
	HK_ASSERT(0x394e9c6c,  i >= 0 && i < m_size );
	return m_data[i];
}

template <typename T>
HK_FORCE_INLINE const T& hkArrayBase<T>::operator[] (int i) const
{
	HK_ASSERT(0x264718f3,  i >= 0 && i < m_size  );
	return m_data[i];
}

template <typename T>
HK_FORCE_INLINE T& hkArrayBase<T>::back()
{
	HK_ASSERT(0x52595f9b, m_size );
	return m_data[ m_size - 1 ];
}

template <typename T>
HK_FORCE_INLINE const T& hkArrayBase<T>::back() const
{
	HK_ASSERT(0x6e984e36, m_size );
	return m_data[ m_size -1 ];
}

template <typename T>
HK_FORCE_INLINE int hkArrayBase<T>::getSize() const
{
	return m_size;
}

template <typename T>
HK_FORCE_INLINE int hkArrayBase<T>::getCapacity() const
{
	return (m_capacityAndFlags & static_cast<int>(CAPACITY_MASK));
}

template <typename T>
HK_FORCE_INLINE int hkArrayBase<T>::getCapacityAndFlags() const
{
    return m_capacityAndFlags;
}

template <typename T>
HK_FORCE_INLINE hkBool hkArrayBase<T>::isEmpty() const
{
	return m_size == 0;
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::_reserveExactly(hkMemoryAllocator& alloc, int n)
{
	if(getCapacity() < n)
	{
		hkArrayUtil::_reserve(alloc, this, n, sizeof(T));
	}
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::_reserve(hkMemoryAllocator& alloc, int n)
{
	const int capacity = getCapacity();
	if( capacity < n)
	{
		int cap2 = 2 * capacity;
		int newSize = (n < cap2) ? cap2 : n;
		hkArrayUtil::_reserve(alloc, this, newSize, sizeof(T));
	}
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::clear()
{
	hkArrayUtil::destruct(m_data, m_size, typename hkIsPodType<T>::type());
	m_size = 0;
}



template <typename T>
HK_FORCE_INLINE void HK_CALL hkArrayBase<T>::copy(T* dst, const T* src, int n)
{
	HK_ASSERT(0x4543e433, (dst <= src) || (src+n <= dst) );
	for(int i = 0; i < n; ++i)
	{
		dst[i] = src[i];
	}
}



template <typename T>
HK_FORCE_INLINE hkArrayBase<T>::hkArrayBase()
	:	m_data(HK_NULL),
		m_size(0),
		m_capacityAndFlags(DONT_DEALLOCATE_FLAG)
{
}

template <typename T>
HK_FORCE_INLINE hkArrayBase<T>& hkArrayBase<T>::copyFromArray(hkMemoryAllocator& alloc, const hkArrayBase<T>& a, hkTypeIsPod)
{
	if( getCapacity() < a.getSize() )
	{
		if( (m_capacityAndFlags & DONT_DEALLOCATE_FLAG) == 0 )
		{
			alloc._bufFree<T>( m_data, getCapacity() );
		}
		int n = a.getSize();
		m_data = alloc._bufAlloc<T>(n);
		m_capacityAndFlags = n;
	}
	m_size = a.getSize();
	copy(m_data, a.m_data, m_size);
	return *this;
}

template <typename T>
HK_FORCE_INLINE hkArrayBase<T>& hkArrayBase<T>::copyFromArray(hkMemoryAllocator& alloc, const hkArrayBase<T>& a, hkTypeIsClass)
{
	int oldSize = m_size;
	int newSize = a.getSize();
	int copiedSize = newSize > oldSize ? oldSize : newSize;

	_reserve(alloc, newSize); // ensure space
	hkArrayUtil::destruct(m_data + newSize, oldSize - newSize, hkTypeIsClass()); // destruct items past the size of a, if any
	copy(m_data, a.m_data, copiedSize); // copy objects into the 'live' part of this array
	hkArrayUtil::constructWithArray(m_data + copiedSize, newSize - copiedSize, a.m_data + copiedSize, hkTypeIsClass()); // and construct the rest
	m_size = newSize;
	return *this;
}

template <typename T>
HK_FORCE_INLINE hkArrayBase<T>::hkArrayBase(T* ptr, int size, int capacity)
	:	m_data(ptr),
		m_size(size),
		m_capacityAndFlags(capacity | DONT_DEALLOCATE_FLAG)
{
	HK_ASSERT2(0x23483be5, size >= 0 && capacity >= 0, "Array size and capacity must be non-negative." );
	HK_ASSERT2(0x4234325a, size <= capacity, "The size must be consistent with the capacity as memory will not be allocated in constructor. Use 'setSize' to allocate memory if inplace capacity too small.");
}

template <typename T>
HK_FORCE_INLINE hkArrayBase<T>::~hkArrayBase()
{
	HK_ASSERT2(0x1129f768, hkIsPodType<T>::result || m_size==0, "Non-POD array elements not destructed");
	HK_ASSERT2(0x1129f769, m_capacityAndFlags&DONT_DEALLOCATE_FLAG, "Array memory not freed");
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::_clearAndDeallocate(hkMemoryAllocator& alloc)
{
	clear();
	if( (m_capacityAndFlags & DONT_DEALLOCATE_FLAG) == 0 )
	{
		alloc._bufFree<T>( m_data, getCapacity() );
	}
	m_data = HK_NULL;
	m_capacityAndFlags = DONT_DEALLOCATE_FLAG;
}

#define HK_COMPUTE_OPTIMIZED_CAPACITY( size, numFreeElemsLeft, shrinkExact ) (shrinkExact) ? (size + numFreeElemsLeft) : hkNextPowerOf2(size + numFreeElemsLeft)

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::_optimizeCapacity( hkMemoryAllocator& alloc, int numFreeElemsLeft, hkBool32 shrinkExact )
{
	int totalCapacity = HK_COMPUTE_OPTIMIZED_CAPACITY(m_size, numFreeElemsLeft, shrinkExact);
	if ( totalCapacity < getCapacity() )
	{
		hkArrayUtil::_reduce( alloc, this, sizeof(T), HK_NULL, totalCapacity );
	}
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::removeAt(int index)
{
	HK_ASSERT(0x63bab20a, index >= 0 && index < m_size);

	hkArrayUtil::destruct(&m_data[index], 1, typename hkIsPodType<T>::type());
	m_size--;
	if( m_size != index )
	{
		hkMemUtil::memCpyOneAligned<sizeof(T), HK_ALIGN_OF(T)>( m_data + index, m_data + m_size );
	}
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::removeAtAndCopy(int index)
{
	HK_ASSERT(0x453a6437, index >= 0 && index < m_size);

	hkArrayUtil::destruct(&m_data[index], 1, typename hkIsPodType<T>::type());
	m_size--;
	hkMemUtil::memCpy<HK_ALIGN_OF(T)>(m_data + index, m_data + index + 1, (m_size-index)*sizeof(T));
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::removeAtAndCopy(int index, int numToRemove)
{
	HK_ASSERT(0x453a6436, numToRemove > 0);
	HK_ASSERT(0x453a6437, index >= 0 && ((index + numToRemove) <= m_size) );

	hkArrayUtil::destruct(m_data + index, numToRemove, typename hkIsPodType<T>::type());
	m_size -= numToRemove;
	hkMemUtil::memCpy<HK_ALIGN_OF(T)>(m_data + index, m_data + index + numToRemove, (m_size-index)*sizeof(T));
}

template <typename T>
HK_FORCE_INLINE int hkArrayBase<T>::indexOf(const T& t, int startIdx, int endIdx) const
{
	if( endIdx < 0 )
	{
		endIdx = m_size;
	}
	for(int i = startIdx; i < endIdx; ++i)
	{
		if( m_data[i] == t )
		{
			return i;
		}
	}
	return -1;
}

template <typename T>
HK_FORCE_INLINE int hkArrayBase<T>::lastIndexOf(const T& t) const
{
	for(int i = m_size-1; i >=0; --i)
	{
		if( m_data[i] == t )
		{
			return i;
		}
	}
	return -1;
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::popBack( int numToRemove )
{
	HK_ASSERT(0x5b57310e, m_size >= numToRemove );
	hkArrayUtil::destruct(m_data + m_size - numToRemove, numToRemove, typename hkIsPodType<T>::type());
	m_size -= numToRemove;
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::_pushBack(hkMemoryAllocator& alloc, const T& t)
{
	if( m_size == getCapacity() )
	{
		HK_ASSERT2( 0x76e453e4, ! ( ( &t >= m_data ) && ( &t < (m_data + m_size) ) ), "hkArrayBase::pushBack can't push back element of same array during resize" );
		hkArrayUtil::_reserveMore(alloc, this, sizeof(T));
	}
	hkArrayUtil::constructWithCopy(m_data + m_size, 1, t, typename hkIsPodType<T>::type());
	m_size++;
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::pushBackUnchecked(const T& t)
{
	HK_ASSERT(0x3a2b4abb, m_size < getCapacity());
	hkArrayUtil::constructWithCopy(m_data + m_size, 1, t, typename hkIsPodType<T>::type());
	m_size++;
}


template <typename T>
HK_FORCE_INLINE hkBool hkArrayBase<T>::tryPushBack(const T& t)
{
	if( m_size < getCapacity() )
	{
		hkArrayUtil::constructWithCopy(m_data + m_size, 1, t, typename hkIsPodType<T>::type());
		m_size++;
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::_setSize(hkMemoryAllocator& alloc, int n)
{
	_reserve(alloc, n);
	hkArrayUtil::destruct(m_data + n, m_size - n, typename hkIsPodType<T>::type());
	hkArrayUtil::construct(m_data + m_size, n - m_size, typename hkIsPodType<T>::type());
	m_size = n;
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::_setSize(hkMemoryAllocator& alloc, int n, const T& fill)
{
	_reserve(alloc, n);
	hkArrayUtil::destruct(m_data + n, m_size - n, typename hkIsPodType<T>::type());
	hkArrayUtil::constructWithCopy(m_data + m_size, n - m_size, fill, typename hkIsPodType<T>::type());
	m_size = n;
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::setSizeUnchecked(int n)
{
	HK_ASSERT(0x39192e68, n <= getCapacity());
	hkArrayUtil::destruct(m_data + n, m_size - n, typename hkIsPodType<T>::type());
	hkArrayUtil::construct(m_data + m_size, n - m_size, typename hkIsPodType<T>::type());
	m_size = n;
}

template <typename T>
HK_FORCE_INLINE T* hkArrayBase<T>::_expandBy( hkMemoryAllocator& alloc, int n )
{
	int oldsize = m_size;
	_reserve(alloc, m_size + n);
	hkArrayUtil::construct(m_data + m_size, n, typename hkIsPodType<T>::type());
	m_size += n;
	return m_data+oldsize;
}

template <typename T>
HK_FORCE_INLINE T& hkArrayBase<T>::_expandOne(hkMemoryAllocator& alloc)
{
	if( m_size == getCapacity() )
	{
		hkArrayUtil::_reserveMore( alloc, this, sizeof(T) );
	}
	hkArrayUtil::construct(m_data + m_size, 1, typename hkIsPodType<T>::type());
	return m_data[m_size++];
}

template <typename T>
HK_FORCE_INLINE T* hkArrayBase<T>::expandByUnchecked( int n )
{
	HK_ASSERT(0x5b7a4705, n >= 0 && m_size+n <= getCapacity() );
	int oldsize = m_size;
	m_size =  oldsize + n;
	hkArrayUtil::construct(m_data + oldsize, m_size - oldsize, typename hkIsPodType<T>::type());
	return m_data+oldsize;
}

template <typename T>
void hkArrayBase<T>::_spliceInto(hkMemoryAllocator& alloc, int index, int numdel, const T* p, int numtoinsert)
{
	HK_ASSERT(0x4cbc67c6, index >= 0 && index <= getSize() );
	HK_ASSERT(0x4cbc67c7, numdel >= 0 && (index+numdel) <= getSize() );
	const int newsize     = numtoinsert + m_size - numdel;
	const int numtomove   = m_size - index - numdel;
	if( newsize > getCapacity() )
	{
		// note double copy from [i:end] not a problem in practice
		_reserve(alloc, newsize);
	}
	hkArrayUtil::destruct(m_data + index, numdel, typename hkIsPodType<T>::type());
	hkMemUtil::memMove(m_data + index + numtoinsert, m_data + index + numdel, numtomove*sizeof(T));
	hkArrayUtil::constructWithArray(m_data + index, numtoinsert, p, typename hkIsPodType<T>::type());
	m_size = newsize;
}

template <typename T>
void hkArrayBase<T>::_insertAt(hkMemoryAllocator& alloc, int index, const T* p, int numtoinsert)
{
	_spliceInto(alloc, index, 0, p, numtoinsert);
}

template <typename T>
void hkArrayBase<T>::_append(hkMemoryAllocator& alloc, const T* a, int numtoinsert)
{
	const int newsize = m_size + numtoinsert;
	if(newsize > getCapacity())
	{
		_reserve(alloc, newsize);
	}
	hkArrayUtil::constructWithArray(m_data + m_size, numtoinsert, a, typename hkIsPodType<T>::type());
	m_size = newsize;
}

template <typename T>
HK_FORCE_INLINE T* hkArrayBase<T>::_expandAt(hkMemoryAllocator& alloc, int index, int numtoinsert)
{
	HK_ASSERT(0x2723cc08,  index >= 0 && index <= m_size );

	const int newsize = numtoinsert + m_size;
	const int numtomove = m_size - index;
	if(newsize > getCapacity())
	{
		// note double copy from [i:end] not a problem in practice
		_reserve(alloc, newsize);
	}
	hkMemUtil::memMove(m_data + index + numtoinsert, m_data + index, numtomove*sizeof(T));
	hkArrayUtil::construct(m_data + index, numtoinsert, typename hkIsPodType<T>::type());
	m_size = newsize;
	return m_data + index;
}

template <typename T>
void hkArrayBase<T>::_insertAt(hkMemoryAllocator& alloc, int i, const T& t)
{
	_insertAt(alloc, i, &t, 1 );
}

template <typename T>
HK_FORCE_INLINE void hkArrayBase<T>::removeAllAndCopy(const T& t)
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
			hkMemUtil::memCpyOneAligned<sizeof(T), HK_ALIGN_OF(T)>( m_data + destination, m_data + source );
			++destination;
		}
		else
		{
			hkArrayUtil::destruct( m_data + source, 1, typename hkIsPodType<T>::type() );
			--m_size;
		}
	}
}


template <typename T>
typename hkArrayBase<T>::iterator hkArrayBase<T>::begin()
{
	return m_data;
}

template <typename T>
typename hkArrayBase<T>::iterator hkArrayBase<T>::end()
{
	return m_data + m_size;
}

template <typename T>
typename hkArrayBase<T>::const_iterator hkArrayBase<T>::begin() const
{
	return m_data;
}

template <typename T>
typename hkArrayBase<T>::const_iterator hkArrayBase<T>::end() const
{
	return m_data + m_size;
}

template <typename T>
void hkArrayBase<T>::_setDataUnchecked(T *ptr, int size, int capacityAndFlags)
{
	m_data = ptr;
	m_size = size;
	m_capacityAndFlags = capacityAndFlags;
}


template <typename T>
void hkArrayBase<T>::_setData(T *ptr, int size, int capacityAndFlags, hkTypeIsPod)
{
	hkArrayBase<T>::_setDataUnchecked(ptr, size, capacityAndFlags);
}

// template <typename T>
// void hkArrayBase<T>::_setData(T *ptr, int size, int capacityAndFlags, hkTypeIsClass)
// {
// 	HK_ASSERT2(0x6335ef93, 0, "setOwnedData can only be called on POD types");
// }

template <typename T>
void hkArrayBase<T>::setDataAutoFree(T* ptr, int size, int capacity)
{
	HK_ASSERT(0x23483be5, size >= 0 );
	HK_ASSERT(0x23483be5, size <= capacity);
	hkArrayBase<T>::_setData(ptr, size, capacity, typename hkIsPodType<T>::type());
}

template <typename T>
void hkArrayBase<T>::setDataUserFree(T* ptr, int size, int capacity)
{
	HK_ASSERT(0x23483be5, size >= 0 );
	HK_ASSERT(0x23483be5, size <= capacity);
	hkArrayBase<T>::_setData(ptr, size, capacity | DONT_DEALLOCATE_FLAG, typename hkIsPodType<T>::type());
}

//
// hkArray
//

template <typename T, typename Allocator>
HK_FORCE_INLINE hkArray<T,Allocator>::~hkArray()
{
	clearAndDeallocate();
}

template <typename T, typename Allocator>
HK_FORCE_INLINE hkArray<T,Allocator>::hkArray(int n)
: hkArrayBase<T>()
{
	hkMemoryAllocator& a = Allocator().get(this);
	const int size = n;
	T* p = n ? a._bufAlloc<T>(n) : HK_NULL;
	int cap = n ? n : hkArrayBase<T>::DONT_DEALLOCATE_FLAG;
	hkArrayBase<T>::_setDataUnchecked( p, size, cap);
	hkArrayUtil::construct( p, size, typename hkIsPodType<T>::type());
}

template <typename T, typename Allocator>
HK_FORCE_INLINE hkArray<T,Allocator>::hkArray(int n, const T& t)
: hkArrayBase<T>()
{
	hkMemoryAllocator& a = Allocator().get(this);
	const int size = n;
	T* p = n ? a._bufAlloc<T>(n) : HK_NULL;
	int cap = n ? n : hkArrayBase<T>::DONT_DEALLOCATE_FLAG;
	hkArrayBase<T>::_setDataUnchecked( p, size, cap);
	hkArrayUtil::constructWithCopy( p, size, t, typename hkIsPodType<T>::type());
}

template <typename T, typename Allocator>
HK_FORCE_INLINE hkArray<T,Allocator>::hkArray(const hkArray<T,Allocator>& arr)
: hkArrayBase<T>()
{
	int n = arr.getSize();
	const int size = n;
	hkMemoryAllocator& a = Allocator().get(this);
	T* p = n ? a._bufAlloc<T>(n) : HK_NULL;
	int cap = n ? n : hkArrayBase<T>::DONT_DEALLOCATE_FLAG;
	hkArrayBase<T>::_setDataUnchecked( p, size, cap);
	hkArrayUtil::constructWithArray( p, size, arr.m_data, typename hkIsPodType<T>::type());
}

template <typename T, typename Allocator>
HK_FORCE_INLINE hkArray<T, Allocator>& hkArray<T, Allocator>::operator= (const hkArrayBase<T>& a)
{
	hkArrayBase<T>::copyFromArray(Allocator().get(this), a, typename hkIsPodType<T>::type());
	return *this;
}

template <typename T, typename Allocator>
HK_FORCE_INLINE hkArray<T, Allocator>& hkArray<T, Allocator>::operator= (const hkArray<T,Allocator>& a)
{
	hkArrayBase<T>::copyFromArray(Allocator().get(this), a, typename hkIsPodType<T>::type());
	return *this;
}

template <typename T, typename Allocator>
void hkArray<T,Allocator>::swap(hkArray<T,Allocator>& a)
{
	HK_ASSERT( 0xf032e612, 0==(hkArrayBase<T>::m_capacityAndFlags&hkArrayBase<T>::DONT_DEALLOCATE_FLAG) || hkArrayBase<T>::m_data == HK_NULL );
	HK_ASSERT( 0xf032e613, 0==(a.m_capacityAndFlags&hkArrayBase<T>::DONT_DEALLOCATE_FLAG) || a.m_data == HK_NULL );

	T* d = hkArrayBase<T>::m_data; // swap data
	hkArrayBase<T>::m_data = a.m_data;
	a.m_data = d;
	int s = hkArrayBase<T>::m_size; // swap size
	hkArrayBase<T>::m_size = a.m_size;
	a.m_size = s;
	int c = hkArrayBase<T>::m_capacityAndFlags; // swap cap
	hkArrayBase<T>::m_capacityAndFlags = a.m_capacityAndFlags;
	a.m_capacityAndFlags = c;
}

//
// Inplace array
//

template <typename T, unsigned N, typename Allocator>
HK_FORCE_INLINE hkInplaceArray<T,N,Allocator>::hkInplaceArray(int size)
	: hkArray<T,Allocator>(m_storage, size, N)
{
}

template <typename T, unsigned N, typename Allocator>
HK_FORCE_INLINE hkInplaceArray<T,N,Allocator>::hkInplaceArray(const hkInplaceArray<T,N,Allocator>& a)
	: hkArray<T,Allocator>(m_storage, 0, N)
{
	*this = a;
}

template <typename T, unsigned N, typename Allocator>
HK_FORCE_INLINE void hkInplaceArray<T,N,Allocator>::optimizeCapacity( int numFreeElemsLeft, hkBool32 shrinkExact )
{
	if( (this->m_capacityAndFlags & hkArray<T,Allocator>::DONT_DEALLOCATE_FLAG) == 0)
	{
		int totalCapacity = HK_COMPUTE_OPTIMIZED_CAPACITY(this->m_size, numFreeElemsLeft, shrinkExact);
		if( totalCapacity < int(N) )
		{
			// reducing to a capacity < N, we can copy back to local storage
			hkArrayUtil::_reduce( typename hkArray<T,Allocator>::AllocatorType().get(this), this, sizeof(T), (char*)(&m_storage[0]), N );
		}
		else if( totalCapacity < this->getCapacity() )
		{
			hkArrayUtil::_reduce( typename hkArray<T,Allocator>::AllocatorType().get(this), this, sizeof(T), HK_NULL, totalCapacity );
		}
	}
}

template <typename T, unsigned N, typename Allocator>
hkArray<T,Allocator>& hkInplaceArray<T,N,Allocator>::operator= (const hkArrayBase<T>& a)
{
	return hkArray<T>::operator=(a);
}

template <typename T, unsigned N, typename Allocator>
hkArray<T,Allocator>& hkInplaceArray<T,N,Allocator>::operator= (const hkInplaceArray<T,N,Allocator>& a)
{
	return hkArray<T,Allocator>::operator=(a);
}

template <typename T, unsigned N, typename Allocator>
hkBool hkInplaceArray<T,N,Allocator>::wasReallocated() const
{
	return this->m_data != m_storage;
}

template <typename T, unsigned N, typename Allocator>
int hkInplaceArray<T,N,Allocator>::stillInplaceUsingMask() const
{
	return hkArray<T,Allocator>::m_capacityAndFlags & hkArrayBase<T>::DONT_DEALLOCATE_FLAG;
}

//
// Inplace array 16
//

template <typename T, unsigned N>
hkArray<T>& hkInplaceArrayAligned16<T,N>::operator= (const hkArrayBase<T>& a)
{
	return hkArray<T>::operator=(a);
}

template <typename T, unsigned N>
hkBool hkInplaceArrayAligned16<T,N>::wasReallocated() const
{
	return hkUlong(this->m_data) != hkUlong(m_storage);
}

template <typename T, unsigned N>
hkArray<T>& hkInplaceArrayAligned16<T,N>::operator= (const hkInplaceArrayAligned16<T,N>& a)
{
	return hkArray<T>::operator=(a);
}

template <typename T, unsigned N>
HK_FORCE_INLINE hkInplaceArrayAligned16<T,N>::hkInplaceArrayAligned16(int size)
	: hkArray<T>( (T*)&m_storage[0], size, N)
{
}

#undef HK_COMPUTE_OPTIMIZED_CAPACITY

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
