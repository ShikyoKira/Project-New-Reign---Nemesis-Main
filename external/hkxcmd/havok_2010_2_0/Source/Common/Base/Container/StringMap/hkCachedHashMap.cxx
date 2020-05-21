// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Common/Base/Container/StringMap/hkCachedHashMap.h>

#define HASH_EMPTY hkUlong(-1)

// Quick description:
// The hash table is stored as a linear list. Initially all keys
// are 0xff (empty). When we insert an element, we jump to the items
// hash and scan forwards until we find the key or we come to an empty entry.
// If the hash function is good and the table is not too crowded, we're
// likely to have good performance and be cache friendly.
// The first third of m_elem are the hashes, the second third the string pointers
// and the final third, the value pointers.

#define ELEM_HASH(i) (m_elem[i].hash)
#define ELEM_KEY(i)  (m_elem[i].key)
#define ELEM_VAL(i)  (m_elem[i].value)

#define NOT_EQUAL(i,hash,key,ops) \
	((ELEM_HASH(i) != hash) || (ops.equal( key, ELEM_KEY(i) ) == 0))
#define EQUAL(i,hash,key,ops) \
	((ELEM_HASH(i) == hash) && (ops.equal( key, ELEM_KEY(i) )     ))


template<typename Operations, typename Allocator>
hkCachedHashMap<Operations, Allocator>::hkCachedHashMap(Operations ops)
:	m_ops(ops)
{
	const int initialCapacity = 16;
	hkMemoryAllocator& mem = Allocator().get(this);
	m_elem = mem._blockAlloc<Elem>( initialCapacity );
	hkString::memSet( m_elem, 0xff, hkSizeOf(Elem) * initialCapacity );
	m_numElems = 0;
	m_hashMod = initialCapacity - 1;
}

template<typename Operations, typename Allocator>
hkCachedHashMap<Operations, Allocator>::~hkCachedHashMap()
{
	hkMemoryAllocator& mem = Allocator().get(this);
	mem._blockFree<Elem>( m_elem, (m_hashMod+1) );
}

template<typename Operations, typename Allocator>
hkCachedHashMap<Operations, Allocator>& hkCachedHashMap<Operations, Allocator>::operator=(const hkCachedHashMap* other)
{
	// Deallocate our storage
	hkMemoryAllocator& mem = Allocator().get(this);
	mem._blockFree<Elem>( m_elem, m_hashMod+1 );
	
	// Copy other storage
	this->m_hashMod = other->m_hashMod;
	this->m_numElems = other->m_numElems;
	m_elem = mem._blockAlloc<Elem>( m_hashMod+1 );

	hkString::memCpy(m_elem, other->m_elem, hkSizeOf(Elem)*(m_hashMod+1) );

	HK_ASSERT( 0x74305156, this->isOk());
	return *this;
}

template<typename Operations, typename Allocator>
typename hkCachedHashMap<Operations, Allocator>::Iterator hkCachedHashMap<Operations, Allocator>::getIterator() const
{
	int i;
	for( i = 0; i <= m_hashMod; ++i )
	{
		if( ELEM_HASH(i) != HASH_EMPTY )
		{
			break;
		}
	}
	return reinterpret_cast<Iterator>( hkUlong(i) );
}

template<typename Operations, typename Allocator>
hkUlong hkCachedHashMap<Operations, Allocator>::getKey(Iterator it) const
{
	int i = static_cast<int>( reinterpret_cast<hkUlong>(it) );
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	return ELEM_KEY(i);
}

template<typename Operations, typename Allocator>
hkUlong hkCachedHashMap<Operations, Allocator>::getValue(Iterator it) const
{
	int i = static_cast<int>( reinterpret_cast<hkUlong>(it) );
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	return ELEM_VAL(i);
}

template<typename Operations, typename Allocator>
void hkCachedHashMap<Operations, Allocator>::setValue(Iterator it, hkUlong val)
{
	int i = static_cast<int>( reinterpret_cast<hkUlong>(it) );
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);
	ELEM_VAL(i) = val;
}

template<typename Operations, typename Allocator>
typename hkCachedHashMap<Operations, Allocator>::Iterator hkCachedHashMap<Operations, Allocator>::getNext( Iterator it ) const
{
	int i = static_cast<int>( reinterpret_cast<hkUlong>(it) );
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod);

	for( i += 1; i <= m_hashMod; ++i )
	{
		if( ELEM_HASH(i) != HASH_EMPTY )
		{
			break;
		}
	}
	return reinterpret_cast<Iterator>( hkUlong(i) );
}

template<typename Operations, typename Allocator>
hkBool hkCachedHashMap<Operations, Allocator>::isValid( Iterator it ) const
{
	int i = static_cast<int>( reinterpret_cast<hkUlong>(it) );
	HK_ASSERT(0x7f305156, i>=0 && i<=m_hashMod+1);
	return i <= m_hashMod;
}

template<typename Operations, typename Allocator>
void hkCachedHashMap<Operations, Allocator>::insert( hkUlong key, hkUlong val )
{
	hkUlong hash = m_ops.hash(key);
	HK_ASSERT(0x3665dd06, hash != HASH_EMPTY);
	// This is quite conservative. We could grow more
	// slowly at the cost of potentially longer searches.
	if( (m_numElems + m_numElems ) > m_hashMod )
	{
		resizeTable(m_hashMod + m_hashMod + 2);
	}
	hkUlong i = hash & m_hashMod;
	while(1) // find free slot
	{
		if(ELEM_HASH(i) == HASH_EMPTY)
		{
			m_numElems += 1;
			break;
		}
		else if( NOT_EQUAL(i, hash, key, m_ops) )
		{
			i = (i+1) & m_hashMod;
		}
		else // overwrite a key
		{
			break;
		}
	}
	
	// insert key,value
	ELEM_HASH(i) = hash;
	ELEM_KEY(i)  = key;
	ELEM_VAL(i)  = val;
}

template<typename Operations, typename Allocator>
typename hkCachedHashMap<Operations, Allocator>::Iterator hkCachedHashMap<Operations, Allocator>::findKey( hkUlong key ) const
{
	hkUlong hash = m_ops.hash(key);
	for(	hkUlong i = hash & m_hashMod;
			ELEM_HASH(i) != HASH_EMPTY;
			i = (i+1) & m_hashMod )
	{
		if( EQUAL(i,hash,key,m_ops) )
		{
			return reinterpret_cast<Iterator>(i);
		}
	}
	return reinterpret_cast<Iterator>( hkUlong(m_hashMod+1) ); // not found
}

template<typename Operations, typename Allocator>
typename hkCachedHashMap<Operations, Allocator>::Iterator hkCachedHashMap<Operations, Allocator>::findOrInsertKey( hkUlong key, hkUlong value )
{
	// reserve space for another element
	if( (m_numElems + m_numElems ) > m_hashMod )
	{
		resizeTable(m_hashMod + m_hashMod + 2);
	}

	hkUlong hash = m_ops.hash(key);
	for(hkUlong i = hash & m_hashMod;
		true;
		i = (i+1) & m_hashMod )
	{
		if( EQUAL(i,hash,key,m_ops) )
		{
			return reinterpret_cast<Iterator>(i);
		}
		else if( ELEM_HASH(i) == HASH_EMPTY )
		{
			ELEM_HASH(i) = hash;
			ELEM_KEY(i) = key;
			ELEM_VAL(i) = value;
			m_numElems += 1;
			return reinterpret_cast<Iterator>(i);
		}
	}
	// not reached
}

template<typename Operations, typename Allocator>
hkResult hkCachedHashMap<Operations, Allocator>::get( hkUlong key, hkUlong* out ) const
{
	Iterator it = findKey(key);
	if( isValid(it) )
	{
		*out = getValue(it);
		return HK_SUCCESS;
	}
	return HK_FAILURE;
}

template<typename Operations, typename Allocator>
hkUlong hkCachedHashMap<Operations, Allocator>::getOrInsert( hkUlong key, hkUlong ifNotFound )
{
	//TODO: examine table directly - save a lookup
	Iterator it = findKey(key);
	if( isValid(it) )
	{
		return getValue(it);
	}
	insert(key,ifNotFound);
	return ifNotFound;
}

template<typename Operations, typename Allocator>
hkUlong hkCachedHashMap<Operations, Allocator>::getWithDefault( hkUlong key, hkUlong def ) const
{
	hkUlong ret = def;
	get(key, &ret);
	return ret;
}

template<typename Operations, typename Allocator>
void hkCachedHashMap<Operations, Allocator>::remove( Iterator it )
{
	hkUlong i = reinterpret_cast<hkUlong>(it);

	// remove it
	--m_numElems;
	ELEM_HASH(i) = HASH_EMPTY;

	// find lowest element of this unbroken run
	hkUlong lo = ( i + m_hashMod ) & m_hashMod;
	while( ELEM_HASH(lo) != HASH_EMPTY )
	{
		lo = ( lo + m_hashMod ) & m_hashMod;
	}
	lo = ( lo + 1 ) & m_hashMod;

	// the slot which has become empty
	hkUlong empty = i;
	
	// sift up, closing any gaps we find
	for( i = (i + 1) & m_hashMod;
		ELEM_HASH(i) != HASH_EMPTY;
		i = (i+1) & m_hashMod )
	{
		hkUlong hmod = ELEM_HASH(i) & m_hashMod;

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
		
		if( ( i >= lo ) && ( hmod > empty ) )
		{
			continue;					
		}
		else if( ( i < empty ) && ( hmod > empty || hmod <= i ) )
		{
			continue;
		}
		else if( /*i > empty && */ ( hmod > empty && hmod < lo ) )
		{
			continue;
		}
		HK_ASSERT(0x3278358f,  i != empty ); // by design
		HK_ASSERT(0x7e3aeff8,  i != lo ); // table became full?!

		// copy up
		ELEM_HASH(empty) = ELEM_HASH(i);
		ELEM_KEY(empty)  = ELEM_KEY(i);
		ELEM_VAL(empty)  = ELEM_VAL(i);
		// source slot is now free
		ELEM_HASH(i) = HASH_EMPTY;
		empty = i;
	}
}

template<typename Operations, typename Allocator>
hkResult hkCachedHashMap<Operations, Allocator>::remove( hkUlong key )
{
	Iterator it = findKey(key);
	if( isValid(it) )
	{
		remove(it);
		return HK_SUCCESS;
	}
	return HK_FAILURE;
}

template<typename Operations, typename Allocator>
void hkCachedHashMap<Operations, Allocator>::resizeTable(int newcap)
{
	HK_ASSERT2(0x1fdccddd,  m_numElems < newcap, "table size is not big enough" );
	HK_ASSERT2(0x18d91741,  HK_NEXT_MULTIPLE_OF(2, newcap) == newcap, "table size should be a power of 2" );

	int oldcap = m_hashMod+1;
	Elem* oldelem = m_elem;
	hkMemoryAllocator& mem = Allocator().get(this);
	m_elem = mem._blockAlloc<Elem>(newcap); // space for values too
	hkString::memSet( m_elem, 0xff, hkSizeOf(Elem) * newcap );
	m_numElems = 0;
	m_hashMod = newcap - 1;

	for( int i = 0; i < oldcap; ++i )
	{
		if( oldelem[i].hash != HASH_EMPTY)
		{
			insert( oldelem[i].key, oldelem[i].value );
		}
	}

	mem._blockFree<Elem>( oldelem, oldcap );
}

template<typename Operations, typename Allocator>
hkBool hkCachedHashMap<Operations, Allocator>::isOk() const
{
	// is count consistent?
	int count = 0;
	int i;
	for( i = 0; i <= m_hashMod; ++i )
	{
		count += ELEM_HASH(i) != HASH_EMPTY;
	}
	HK_ASSERT(0x11c13481,  count == m_numElems );

	// is element reachable from its hash?
	for( i = 0; i <= m_hashMod; ++i )
	{
		if( ELEM_HASH(i) != HASH_EMPTY )
		{
			hkUlong hash = ELEM_HASH(i);
			hkUlong key = ELEM_KEY(i);
			hkUlong j = hash & m_hashMod;
			while( NOT_EQUAL(j, hash, key, m_ops) )
			{
				j = (j + 1) & m_hashMod;
				HK_ASSERT(0x5ec0df1b,  ELEM_HASH(j) != HASH_EMPTY );
			}
		} 
	}
	return true;
}

template<typename Operations, typename Allocator>
void hkCachedHashMap<Operations, Allocator>::clear()
{
	for( int i = 0; i < m_hashMod+1; ++i )
	{
		m_elem[i].hash = HASH_EMPTY;
	}
	m_numElems = 0;
}

template<typename Operations, typename Allocator>
void hkCachedHashMap<Operations, Allocator>::swap( hkCachedHashMap& other )
{
	Elem* te = m_elem;
	hkUlong tn = m_numElems;
	hkUlong th = m_hashMod;
	m_elem = other.m_elem;
	m_numElems = other.m_numElems;
	m_hashMod = other.m_hashMod;
	other.m_elem = te;
	other.m_numElems = static_cast<int>(tn);
	other.m_hashMod = static_cast<int>(th);
}

template<typename Operations, typename Allocator>
void hkCachedHashMap<Operations, Allocator>::merge( const hkCachedHashMap& other )
{
	int ohm = other.m_hashMod;
	for( int i = 0; i <= ohm; ++i )
	{
		if( other.m_elem[i].hash != HASH_EMPTY )
		{
			hkUlong key = other.m_elem[i].key;
			hkUlong val = other.m_elem[i].value;
			insert( key, val );
		} 
	}
}

template<typename Operations, typename Allocator>
int hkCachedHashMap<Operations, Allocator>::getCapacity() const
{
	return m_hashMod + 1;
}

template<typename Operations, typename Allocator>
void hkCachedHashMap<Operations, Allocator>::reserve( int numElements )
{
	int minCap = numElements * 2; // max 50% occupancy
	int cap = 8;
	while (cap < minCap)
	{
		cap *= 2;
	}
	if( cap > m_hashMod+1 ) // only grow
	{
		resizeTable( cap );
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
