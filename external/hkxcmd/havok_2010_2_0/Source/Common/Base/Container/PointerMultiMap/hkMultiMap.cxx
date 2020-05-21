// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#if 0
	// Debugging for iterators. The most common problem is using an iterator after the map has been modified.
	// We can catch resizes easily by watching m_hashMod, but to do this properly we need an int m_serial which
	// gets incremented on every mutate. (otherwise we may miss a delete&insert, as m_hashMod and m_size remain constant)
	struct Iterator { Iterator(int i, int s) : index(i), serial(s) {} int index; int serial; };
    #define HK_MULTI_MAP_INDEX_TO_ITERATOR(i) Iterator(i,m_serial)
    inline int HK_MULTI_MAP_ITERATOR_TO_INDEX(Iterator it) const
	{
		HK_ASSERT2(0x1be297d6, it.hashMod == m_hashMod, "iterator invalidated" );
		return it.index;
	}
#endif

// Quick description:
// The hash table is stored as a linear list. Initially all keys
// are zero (empty). When we insert an element, we jump to the items
// hash and scan forwards until we find the key or we come to a zero entry.
// If the hash function is good and the table is not too crowded, we're
// likely to have good performance and be cache friendly. Values for index i
// are stored at m_elem[ i + size of table ]. We should consider storing the
// values directly following the keys - that would save a cache miss if
// there aren't too many hash collisions.

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
hkMultiMap<KEY,VAL,OPS,ALLOC>::hkMultiMap()
{
	const int initialCapacity = 16;

	hkMemoryRouter::Allocator& alloc = ALLOC().get(this);
	m_elem = (Pair*)alloc.blockAlloc( sizeof(Pair) * initialCapacity);

	m_numElems = 0;
	m_hashMod = initialCapacity - 1;

	// clear the table
	clear();
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
hkMultiMap<KEY,VAL,OPS,ALLOC>::hkMultiMap(int numElements)
: m_numElems(0)
{
	// Make sure that the actual table size is not going to be less than twice the current number of elements
	HK_ASSERT(0x4d0c5315, numElements > 0);
	// Reserve 3 times as much space as the expected number of elements
	numElements *= 3;
	// The size needs to be a power of two
	int size = 4;
	while (size < numElements) { size *= 2; }

	hkMemoryRouter::Allocator& alloc = ALLOC().get(this);
	m_elem = (Pair*)alloc.blockAlloc(sizeof(Pair) * size);
	m_hashMod = size - 1;

	// clear the table
	clear();
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
int hkMultiMap<KEY,VAL,OPS,ALLOC>::findNumEntries(KEY key) const
{
    int numFound = 0;
	for( unsigned int i = OPS::hash(key, m_hashMod);
		OPS::isValid(m_elem[i].key);
		i = (i+1) & m_hashMod)
	{
		if (OPS::equal(m_elem[i].key, key))
		{
            numFound ++;
		}
	}
    return numFound;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
int hkMultiMap<KEY,VAL,OPS,ALLOC>::findNumEntries(KEY key, VAL val) const
{
    int numFound = 0;
	for( unsigned int i = OPS::hash(key, m_hashMod);
		OPS::isValid(m_elem[i].key);
		i = (i+1) & m_hashMod)
	{
		if (OPS::equal(m_elem[i].key, key) && m_elem[i].val == val)
		{
            numFound ++;
		}
	}
    return numFound;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
int hkMultiMap<KEY,VAL,OPS,ALLOC>::removeAll( KEY key)
{
    int numRemoved = 0;

	unsigned hash = OPS::hash(key, m_hashMod);

    {
		unsigned i = hash;
		while( OPS::isValid( m_elem[i].key ))
		{
			if( OPS::equal( m_elem[i].key, key ) )
			{
				remove(HK_MULTI_MAP_INDEX_TO_ITERATOR(i));
				numRemoved++;
				// Restart
				i = hash;
			}
			else
			{
				i = (i+1) & m_hashMod;
			}
		}	
    }

	return numRemoved;
}


template < typename KEY, typename VAL, typename OPS, typename ALLOC >
hkMultiMap<KEY,VAL,OPS,ALLOC>::hkMultiMap(void* ptr, int sizeInBytes)
{
	int maxKeys = unsigned(sizeInBytes) / (sizeof(Pair)); // unsigned div = shift
	HK_ASSERT( 0x6129a7bc, maxKeys >= 8 );
	HK_ASSERT( 0x549309be, isPower2(maxKeys) );
	m_elem = static_cast<Pair*>(ptr);
	m_numElems = DONT_DEALLOCATE_FLAG;
	m_hashMod = maxKeys - 1;
	HK_ASSERT( 0x549309bf, (maxKeys*hkSizeOf(Pair)) == sizeInBytes );
	
	for (int i = 0; i < maxKeys; i++)
	{
		OPS::invalidate( m_elem[i].key );
	}
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
void hkMultiMap<KEY,VAL,OPS,ALLOC>::clear()
{
	int initialCapacity = m_hashMod+1;
	for (int i = 0; i < initialCapacity; i++)
	{
		OPS::invalidate( m_elem[i].key );
	}
	m_numElems = 0 | (m_numElems & static_cast<int>(DONT_DEALLOCATE_FLAG));
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
void hkMultiMap<KEY,VAL,OPS,ALLOC>::swap( hkMultiMap& other )
{
	Pair* te = m_elem;
	hkUlong tn = m_numElems;
	hkUlong th = m_hashMod;
	m_elem = other.m_elem;
	m_numElems = other.m_numElems;
	m_hashMod = other.m_hashMod;
	other.m_elem = te;
	other.m_numElems = static_cast<int>(tn);
	other.m_hashMod = static_cast<int>(th);
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
hkMultiMap<KEY,VAL,OPS,ALLOC>::~hkMultiMap()
{
	if( (m_numElems & DONT_DEALLOCATE_FLAG) == 0)
	{
		hkMemoryRouter::Allocator& alloc = ALLOC().get(this);
		alloc.blockFree(m_elem, sizeof(Pair) * (m_hashMod+1));
	}
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
void hkMultiMap<KEY,VAL,OPS,ALLOC>::insert( KEY key, VAL val )
{
	HK_ASSERT2(0x19291575, OPS::isValid(key), "pointer map keys must not be the empty value");
	// This is quite conservative. We could grow more
	// slowly at the cost of potentially longer searches.
	int numElems = m_numElems & static_cast<int>(NUM_ELEMS_MASK);
	if( (numElems + numElems) > m_hashMod )
	{
		resizeTable(m_hashMod + m_hashMod + 2);
	}

	unsigned i;
	for( i = OPS::hash(key, m_hashMod);
		OPS::isValid(m_elem[i].key);
		i = (i+1) & m_hashMod )
	{
        // Just look for a free slot
	}

    // A new element is added
    m_numElems ++;

	// insert key,value
	m_elem[i].key = key;
	m_elem[i].val = val;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
typename hkMultiMap<KEY,VAL,OPS,ALLOC>::Iterator hkMultiMap<KEY,VAL,OPS,ALLOC>::findKey( KEY key ) const
{
	for( unsigned i = OPS::hash(key, m_hashMod);
		OPS::isValid( m_elem[i].key );
		i = (i+1) & m_hashMod)
	{
		if( OPS::equal( m_elem[i].key, key ) )
		{
            return HK_MULTI_MAP_INDEX_TO_ITERATOR(i); // found
		}
	}
    return HK_MULTI_MAP_INDEX_TO_ITERATOR(m_hashMod+1); // not found
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
typename hkMultiMap<KEY,VAL,OPS,ALLOC>::Iterator hkMultiMap<KEY,VAL,OPS,ALLOC>::findKeyWithValue( KEY key, VAL val ) const
{
	for( unsigned i = OPS::hash(key, m_hashMod);
		OPS::isValid( m_elem[i].key );
		i = (i+1) & m_hashMod)
	{
		const Pair& pair = m_elem[i];
		if( OPS::equal( pair.key, key ) &&
			pair.val == val)
		{
			return HK_MULTI_MAP_INDEX_TO_ITERATOR(i); // found
		}
	}
	return HK_MULTI_MAP_INDEX_TO_ITERATOR(m_hashMod+1); // not found
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
typename hkMultiMap<KEY,VAL,OPS,ALLOC>::Iterator hkMultiMap<KEY,VAL,OPS,ALLOC>::findOrInsertKey( KEY key, VAL val )
{
	HK_ASSERT2(0x19291575, OPS::isValid(key), "pointer map keys must not be the empty value");

	// reserve space for another element
	if( (m_numElems + 1 + m_numElems + 1) > m_hashMod )
	{
		resizeTable(m_hashMod + m_hashMod + 2);
	}

	unsigned i;
	for( i = OPS::hash(key, m_hashMod);
		true;
		i = (i+1) & m_hashMod)
	{
		if( OPS::equal( m_elem[i].key, key ) )
		{
            return HK_MULTI_MAP_INDEX_TO_ITERATOR(i); // found
		}
		else if( OPS::isValid(m_elem[i].key) == false ) // end of chain, insert
		{
			m_elem[i].key = key;
			m_elem[i].val = val;
			m_numElems += 1;
            return HK_MULTI_MAP_INDEX_TO_ITERATOR( i ); // new elem
		}
	}
	// notreached
}


template < typename KEY, typename VAL, typename OPS, typename ALLOC >
hkResult hkMultiMap<KEY,VAL,OPS,ALLOC>::get( KEY key, VAL* out ) const
{
	Iterator it = findKey(key);
	if( isValid(it) )
	{
		*out = getValue(it);
		return HK_SUCCESS;
	}
	return HK_FAILURE;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
VAL hkMultiMap<KEY,VAL,OPS,ALLOC>::getWithDefault( KEY key, VAL def ) const
{
	for( unsigned i = OPS::hash(key, m_hashMod);
		OPS::isValid( m_elem[i].key );
		i = (i+1) & m_hashMod)
	{
		if( OPS::equal( m_elem[i].key, key ) )
		{
			return m_elem[i].val;
		}
	}
	return def;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
void hkMultiMap<KEY,VAL,OPS,ALLOC>::remove( Iterator it )
{
	HK_ASSERT(0x5a6d564c, isValid(it));
    unsigned i = HK_MULTI_MAP_ITERATOR_TO_INDEX(it);

	// remove it
	--m_numElems;
	OPS::invalidate( m_elem[i].key );

	// find lowest element of this unbroken run
	unsigned lo = ( i + m_hashMod ) & m_hashMod;
	while( OPS::isValid( m_elem[lo].key ) )
	{
		lo = ( lo + m_hashMod ) & m_hashMod;
	}
	lo = ( lo + 1 ) & m_hashMod;

	// the slot which has become empty
	unsigned empty = i;

	// shift up, closing any gaps we find
	for(i = (i + 1) & m_hashMod;
		OPS::isValid( m_elem[i].key ); // end of run
		i = (i + 1) & m_hashMod )
	{
		unsigned hash = OPS::hash( m_elem[i].key, m_hashMod );

		// Three cases to consider here.
		// a) The normal case where lo <= empty < i.
		// b) The case where i has wrapped around.
		// c) The case where both i and empty have wrapped around.
		// The initial case will be a. (Or b if the last slot is freed).
		// and may progress to b, and finally c.
		// The algorithm will terminate before 'i' reaches 'lo'
		// otherwise the table would have no free slots.

		// 'normal'      'i wrapped'   'i and empty wrapped'
		// ===== lo      ===== i       ===== empty
		// ===== empty   ===== lo      ===== i
		// ===== i       ===== empty   ===== lo


		if( ( i >= lo ) && ( hash > empty ) )
		{
			continue;
		}
		else if( ( i < empty ) && ( hash > empty || hash <= i ) )
		{
			continue;
		}
		else if( /*i > empty && */ ( hash > empty && hash < lo ) )
		{
			continue;
		}
		HK_ASSERT(0x45e3d455,  i != empty ); // by design
		HK_ASSERT(0x5ef0d6c0,  i != lo ); // table became full?!

		// copy up
		m_elem[empty].key = m_elem[i].key;
		m_elem[empty].val = m_elem[i].val;
		// source slot is now free
		OPS::invalidate( m_elem[i].key );
		empty = i;
	}
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
hkResult hkMultiMap<KEY,VAL,OPS,ALLOC>::remove( KEY key )
{
	Iterator it = findKey(key);
	if( isValid(it) )
	{
		remove(it);
		return HK_SUCCESS;
	}
	return HK_FAILURE;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
hkResult hkMultiMap<KEY,VAL,OPS,ALLOC>::remove( KEY key, VAL val )
{
	Iterator it = findKeyWithValue(key, val);
	if( isValid(it) )
	{
		remove(it);
		return HK_SUCCESS;
	}
	return HK_FAILURE;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
void hkMultiMap<KEY,VAL,OPS,ALLOC>::reserve( int numElements )
{
	// Make sure that the actual table size is not going to be less than twice the current number of elements
	HK_ASSERT(0x4d0c5314, numElements > 0 && (m_numElems & static_cast<int>(NUM_ELEMS_MASK)) * 2 < numElements * 3 );
	// Reserve 3 times as much space as the expected number of elements
	numElements *= 3;
	// The size needs to be a power of two
	int size = 4;
	while (size < numElements) { size *= 2; }

	resizeTable( size );
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
void hkMultiMap<KEY,VAL,OPS,ALLOC>::resizeTable(int newcap)
{
	HK_ASSERT2(0x57c91b4a,  m_numElems < newcap, "table size is not big enough" );
	HK_ASSERT2(0x6c8f2576,  HK_NEXT_MULTIPLE_OF(2, newcap) == newcap, "table size should be a power of 2" );

	int dontDeallocate = m_numElems & static_cast<int>(DONT_DEALLOCATE_FLAG);
	int oldcap = m_hashMod+1;
	Pair* oldelem = m_elem;

	hkMemoryRouter::Allocator& alloc = ALLOC().get(this);
	m_elem = (Pair*)alloc.blockAlloc(sizeof(Pair) * newcap);
	for (int i = 0; i < newcap; i++)
	{
		OPS::invalidate( m_elem[i].key );
	}
	m_numElems = 0;
	m_hashMod = newcap - 1;

	for( int i = 0; i < oldcap; ++i )
	{
		if( OPS::isValid( oldelem[i].key ) )
		{
			insert( oldelem[i].key, oldelem[i].val );
		}
	}

	if (dontDeallocate == 0)
	{
		alloc.blockFree(oldelem, sizeof(Pair) * oldcap);
	}
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
hkBool hkMultiMap<KEY,VAL,OPS,ALLOC>::isOk() const
{
	// is count consistent?
	int count = 0;
	int i;
	for( i = 0; i <= m_hashMod; ++i )
	{
		count += OPS::isValid( m_elem[i].key );
	}
	HK_ASSERT(0x26f64ec4, count == (m_numElems & static_cast<int>(NUM_ELEMS_MASK)) );

	// is element reachable from its hash?
	for( i = 0; i <= m_hashMod; ++i )
	{
		if( OPS::isValid(m_elem[i].key) )
		{
			unsigned j = OPS::hash( m_elem[i].key, m_hashMod );
			while( OPS::equal(m_elem[j].key, m_elem[i].key) == false )
			{
				j = (j + 1) & m_hashMod;
				HK_ASSERT(0x4f6528df,  OPS::isValid(m_elem[j].key) );
			}
		}
	}
	return true;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
int hkMultiMap<KEY,VAL,OPS,ALLOC>::getSizeInBytesFor(int numOfKeys)
{
	// adjust the number to the power of 2
	int requiredNum = 4;
	while (requiredNum < numOfKeys) { requiredNum *= 2; }
	// calculate the table size in bytes:
	// (key,value) * (num keys) * (fill factor)
	return (hkSizeOf(Pair)) * requiredNum * 2;
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
int hkMultiMap<KEY,VAL,OPS,ALLOC>::getMemSize() const
{
	return (m_hashMod + 1) * hkSizeOf(Pair);
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
