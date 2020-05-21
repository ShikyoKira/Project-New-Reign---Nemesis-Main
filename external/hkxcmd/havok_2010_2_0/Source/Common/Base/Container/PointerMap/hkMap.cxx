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
	#define HK_MAP_INDEX_TO_ITERATOR(i) Iterator(i,m_serial)
	inline int HK_MAP_ITERATOR_TO_INDEX(Iterator it) const
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
// likely to have good performance and be cache friendly.

static const int s_minimumCapacity = 8;

template < typename KEY, typename VAL, typename OPS >
hkMapBase<KEY,VAL,OPS>::hkMapBase(void* ptr, int sizeInBytes)
{
	init(ptr, sizeInBytes);
}

template < typename KEY, typename VAL, typename OPS >
void hkMapBase<KEY,VAL,OPS>::init(void* ptr, int sizeInBytes)
{
	int maxKeys = unsigned(sizeInBytes) / (sizeof(Pair)); // unsigned div = shift
	HK_ASSERT( 0x6129a7bc, maxKeys >= s_minimumCapacity );
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

template < typename KEY, typename VAL, typename OPS >
void hkMapBase<KEY,VAL,OPS>::clear()
{
	int capacity = m_hashMod+1;
	for (int i = 0; i < capacity; i++)
	{
		OPS::invalidate( m_elem[i].key ); 
	} 
	m_numElems = 0 | (m_numElems & static_cast<int>(DONT_DEALLOCATE_FLAG));
}

template < typename KEY, typename VAL, typename OPS >
void hkMapBase<KEY,VAL,OPS>::clearAndDeallocate(hkMemoryAllocator& alloc)
{
	clear();
	if( (m_numElems & DONT_DEALLOCATE_FLAG) == 0 )
	{
		alloc.blockFree( m_elem, sizeof(Pair)* (m_hashMod+1) );
		HK_ON_DEBUG( m_numElems |= DONT_DEALLOCATE_FLAG );
	}
	m_elem = HK_NULL; 
	m_numElems = 0; 
	m_hashMod = -1;
}

template < typename KEY, typename VAL, typename OPS >
hkMapBase<KEY,VAL,OPS>::~hkMapBase()
{
	HK_ASSERT2(0x2a1ebb4e, (m_numElems & DONT_DEALLOCATE_FLAG) || (m_elem == HK_NULL), "memory not freed" );
}

template < typename KEY, typename VAL, typename OPS >
hkBool32 hkMapBase<KEY,VAL,OPS>::insert( hkMemoryAllocator& alloc, KEY key, VAL val )
{
	HK_ASSERT2(0x19291575, OPS::isValid(key), "pointer map keys must not be the empty value");
	// This is quite conservative. We could grow more
	// slowly at the cost of potentially longer searches.
	int numElems = m_numElems & static_cast<int>(NUM_ELEMS_MASK);
	if( (numElems + numElems) > m_hashMod )
	{
		resizeTable(alloc, m_hashMod + m_hashMod + 2);
	}
	
	unsigned i;
	hkBool32 isNewKey = true;
	for( i = OPS::hash(key, m_hashMod);
		OPS::isValid(m_elem[i].key);
		i = (i+1) & m_hashMod )
	{
		// find free slot
		if( OPS::equal( m_elem[i].key, key ) )
		{
			isNewKey = false;
			break;
		}
	}

	// dont increment m_numElems if overwriting.
	m_numElems += isNewKey;

	// insert key,value
	m_elem[i].key = key;
	m_elem[i].val = val;
	return isNewKey;
}

template < typename KEY, typename VAL, typename OPS >
typename hkMapBase<KEY,VAL,OPS>::Iterator hkMapBase<KEY,VAL,OPS>::findKey( KEY key ) const
{
	if( m_hashMod > 0 )
	{
		for( unsigned i = OPS::hash(key, m_hashMod);
			OPS::isValid( m_elem[i].key );	
			i = (i+1) & m_hashMod)
		{
			if( OPS::equal( m_elem[i].key, key ) )
			{
				return HK_MAP_INDEX_TO_ITERATOR(i); // found
			}
		}
	}
	return HK_MAP_INDEX_TO_ITERATOR(m_hashMod+1); // not found
}

template < typename KEY, typename VAL, typename OPS >
typename hkMapBase<KEY,VAL,OPS>::Iterator hkMapBase<KEY,VAL,OPS>::findOrInsertKey( hkMemoryAllocator& alloc, KEY key, VAL val )
{
	HK_ASSERT2(0x19291575, OPS::isValid(key), "pointer map keys must not be the empty value");

	// reserve space for another element
	int numElems = m_numElems & static_cast<int>(NUM_ELEMS_MASK);
	if( (numElems + numElems) > m_hashMod )
	{
		resizeTable(alloc, m_hashMod + m_hashMod + 2);
	}

	unsigned i;
	for( i = OPS::hash(key, m_hashMod);
		true;
		i = (i+1) & m_hashMod)
	{
		if( OPS::equal( m_elem[i].key, key ) )
		{
			return HK_MAP_INDEX_TO_ITERATOR(i); // found
		}
		else if( OPS::isValid(m_elem[i].key) == false ) // end of chain, insert
		{
			m_elem[i].key = key;
			m_elem[i].val = val;
			m_numElems += 1;
			return HK_MAP_INDEX_TO_ITERATOR( i ); // new elem
		}
	}
	// notreached
}


template < typename KEY, typename VAL, typename OPS >
hkResult hkMapBase<KEY,VAL,OPS>::get( KEY key, VAL* out ) const
{
	Iterator it = findKey(key);
	if( isValid(it) )
	{
		*out = getValue(it);
		return HK_SUCCESS;
	}
	return HK_FAILURE;
}

template < typename KEY, typename VAL, typename OPS >
VAL hkMapBase<KEY,VAL,OPS>::getWithDefault( KEY key, VAL def ) const
{
	if( m_hashMod > 0 )
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
	}
	return def;
}







template < typename KEY, typename VAL, typename OPS >
void hkMapBase<KEY,VAL,OPS>::remove( Iterator it )
{
	HK_ASSERT(0x5a6d564c, isValid(it));
	HK_ASSERT(0x5a6d564d, getSize() > 0);
	unsigned i = HK_MAP_ITERATOR_TO_INDEX(it);

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

template < typename KEY, typename VAL, typename OPS >
hkResult hkMapBase<KEY,VAL,OPS>::remove( KEY key )
{
	Iterator it = findKey(key);
	if( isValid(it) )
	{
		remove(it);
		return HK_SUCCESS;
	}
	return HK_FAILURE;	
}

template < typename KEY, typename VAL, typename OPS >
void hkMapBase<KEY,VAL,OPS>::reserve( hkMemoryAllocator& alloc, int numElements )
{
	// Make sure that the actual table size is not going to be less than twice the current number of elements
	HK_ASSERT(0x4d0c5314, numElements >= 0 && (m_numElems & static_cast<int>(NUM_ELEMS_MASK)) * 2 <= numElements * 3 );
	int minCap = numElements * 2;
	int cap = s_minimumCapacity;
	while (cap < minCap) { cap *= 2; }
	
	resizeTable( alloc, cap );
}

template < typename KEY, typename VAL, typename OPS >
void hkMapBase<KEY,VAL,OPS>::resizeTable(hkMemoryAllocator& alloc, int newcap)
{
	newcap = hkMath::max2( newcap, s_minimumCapacity );
	HK_ASSERT2(0x57c91b4a,  m_numElems < newcap, "table size is not big enough" );
	HK_ASSERT2(0x6c8f2576,  HK_NEXT_MULTIPLE_OF(2, newcap) == newcap, "table size should be a power of 2" );
	
	hkBool32 dontDeallocate = m_numElems & static_cast<int>(DONT_DEALLOCATE_FLAG);
	int oldcap = m_hashMod+1;
	Pair* oldelem = m_elem;
	m_elem = static_cast<Pair*>( alloc.blockAlloc( sizeof(Pair)*newcap ) ); // space for values too
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
			insert( alloc, oldelem[i].key, oldelem[i].val );
		}
	}

	if (dontDeallocate == 0)
	{
		alloc.blockFree( oldelem, sizeof(Pair)*oldcap );
	}
}

template < typename KEY, typename VAL, typename OPS >
hkBool hkMapBase<KEY,VAL,OPS>::isOk() const
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

template < typename KEY, typename VAL, typename OPS >
void hkMapBase<KEY,VAL,OPS>::setOwnedData(void* ptr, int size, int capacity)
{
	HK_ASSERT(0x27af34e5, size >= 0 );
	HK_ASSERT(0x29d4eef2, size <= capacity );
	m_elem = static_cast<Pair*>(ptr);
	m_numElems = size | DONT_DEALLOCATE_FLAG;
	m_hashMod = capacity - 1;
}


template < typename KEY, typename VAL, typename OPS >
int hkMapBase<KEY,VAL,OPS>::getSizeInBytesFor(int numOfKeys)
{
	// adjust the number to the power of 2
	int numSlots = numOfKeys * 2; // half full
	int cap;
	for( cap = s_minimumCapacity; cap < numSlots; cap *= 2 )
	{
		// double until sufficient capacity
	}
	return cap * hkSizeOf(Pair);
}

template < typename KEY, typename VAL, typename OPS >
int hkMapBase<KEY,VAL,OPS>::getMemSize() const
{ 
	return (m_hashMod + 1) * hkSizeOf(Pair);
}

template < typename KEY, typename VAL, typename OPS, typename ALLOC >
void hkMap<KEY,VAL,OPS,ALLOC>::swap( hkMap& other )
{
	typename hkMapBase<KEY,VAL,OPS>::Pair* te = hkMapBase<KEY,VAL,OPS>::m_elem;
	hkUlong tn = hkMapBase<KEY,VAL,OPS>::m_numElems;
	hkUlong th = hkMapBase<KEY,VAL,OPS>::m_hashMod;
	hkMapBase<KEY,VAL,OPS>::m_elem = other.m_elem;
	hkMapBase<KEY,VAL,OPS>::m_numElems = other.m_numElems;
	hkMapBase<KEY,VAL,OPS>::m_hashMod = other.m_hashMod;
	other.m_elem = te;
	other.m_numElems = static_cast<int>(tn);
	other.m_hashMod = static_cast<int>(th);
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
