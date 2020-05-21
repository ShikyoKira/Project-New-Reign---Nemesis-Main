/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template<typename KeyType>
void hkObjectCache<KeyType>::removeObjectByKey( KeyType key )
{
	removeActiveObjectByIndex( getIndexByKey( key ) );
}

template<typename KeyType>
void hkObjectCache<KeyType>::removeObjectByReference( hkReferencedObject* object )
{
	removeActiveObjectByIndex( getIndexByReference( object ) );
}

template<typename KeyType>
hkUint32 hkObjectCache<KeyType>::getCurrentSize() const
{
	return m_currentSize;
}

template<typename KeyType>
hkUint32 hkObjectCache<KeyType>::getMaxSize() const
{
	return m_maxSize;
}

template<typename KeyType>
hkObjectCache<KeyType>::hkObjectCache( hkUint32 size )
: m_maxSize( size )
, m_currentSize( 0 )
{
}

template<typename KeyType>
void hkObjectCache<KeyType>::addObject( KeyType key, hkReferencedObject* object, hkUint32 size, int usage )
{
	const int index = getIndexByKey( key );
	HK_ASSERT2( 0x563ad718, index <= 0, "Data for key already in cache." );
	// If there is an old object for this key in the preserved array, we remove it.
	if ( index < 0 )
	{
		const int preservedIndex = -index - 1;
		m_currentSize -= m_preservedCacheInfo[preservedIndex].m_size;
		m_preservedCacheInfo[preservedIndex].m_object->removeReference();
		m_preservedCacheInfo.removeAtAndCopy( preservedIndex );
	}
	HK_ASSERT2( 0x563ad732, usage >= 0, "Cannot have a negative usage count." );
	HK_WARN_ON_DEBUG_IF( size == 0, 0x563ad75b, "Adding an object of zero size to the cache." );
	object->addReference();
	if ( usage > 0 )
	{
		ActiveCacheInfo& newInfo = m_activeCacheInfo.expandOne();
		{
			newInfo.m_key = key;
			newInfo.m_object = object;
			newInfo.m_usageCount = usage;
			newInfo.m_size = size;
		}
	}
	else if ( size <= m_maxSize )
	{
		purgeEnoughFor( size );
		PreservedCacheInfo& newInfo = m_preservedCacheInfo.expandOne();
		{
			newInfo.m_key = key;
			newInfo.m_object = object;
			newInfo.m_size = size;
		}
		m_currentSize += size;
	}
	HK_WARN_ON_DEBUG_IF( ( usage == 0 ) && ( size > m_maxSize ), 0x3e316892, "Tried to preserve too large an object in the cache." );
}

template<typename KeyType>
hkReferencedObject* hkObjectCache<KeyType>::getObject( KeyType key )
{
	const int index = getIndexByKey( key );
	if ( index > 0 )
	{
		// The body is active, so increase its usage count.
		ActiveCacheInfo& info = m_activeCacheInfo[index - 1];
		++info.m_usageCount;
		return info.m_object;
	}
	else if ( index < 0 )
	{
		// The object is preserved but needs to be reactivated.
		const int preservedIndex = -index - 1;
		PreservedCacheInfo& info = m_preservedCacheInfo[ preservedIndex ];
		ActiveCacheInfo& newInfo = m_activeCacheInfo.expandOne();
		{
			// Copy the common parts.
			(CacheInfo&) newInfo = (CacheInfo&) info;
			newInfo.m_usageCount = 1;
		}
		HK_ASSERT2( 0x563ad719, m_currentSize >= info.m_size, "Cache hasn't recorded size correctly." );
		m_currentSize -= info.m_size;
		m_preservedCacheInfo.removeAtAndCopy( preservedIndex );
		return newInfo.m_object;
	}
	else
	{
		return HK_NULL;
	}
}

template<typename KeyType>
void hkObjectCache<KeyType>::removeActiveObjectByIndex( int index )
{
	HK_ASSERT2( 0x563ad720, index != 0, "Data for key not found in cache." );
	HK_ASSERT2( 0x563ad721, index >= 0, "Key was removed too many times." );
	const int activeIndex = index - 1;
	ActiveCacheInfo& info = m_activeCacheInfo[activeIndex];
	HK_ASSERT2( 0x563ad722, info.m_usageCount > 0, "Usage count should never be zero." );
	if ( info.m_usageCount == 1 )
	{
		// The object is no longer active, so it should be preserved.
		// But only if we can fit it in.
		if ( info.m_size <= m_maxSize )
		{
			// Free enough space.
			purgeEnoughFor( info.m_size );
			// Add the new entry.
			PreservedCacheInfo& newInfo = m_preservedCacheInfo.expandOne();
			{
				// Copy the common data.
				(CacheInfo&) newInfo = (CacheInfo&) info;
			}
			m_currentSize += newInfo.m_size;
		}
		else
		{
			// There wasn't enough room for this object in the cache, so we can't preserve it.
			m_activeCacheInfo[activeIndex].m_object->removeReference();
		}
		// Delete the old entry.
		m_activeCacheInfo.removeAt(activeIndex);
	}
	else
	{
		--info.m_usageCount;
	}
}

template<typename KeyType>
void hkObjectCache<KeyType>::removePreservedObjectByIndex( int index )
{
	HK_ASSERT2( 0x563ad720, index != 0, "Data for key not found in cache." );
	HK_ASSERT2( 0x563ad721, index <= 0, "Cannot call this method on an active object." );
	const int preservedIndex = -index - 1;
	PreservedCacheInfo& info = m_preservedCacheInfo[preservedIndex];
	info.m_object->removeReference();
	m_currentSize -= info.m_size;
	m_preservedCacheInfo.removeAtAndCopy( preservedIndex );
}

template<typename KeyType>
hkInt32 hkObjectCache<KeyType>::getIndexByKey( KeyType key ) const
{
	{
		const int numActiveEntries = m_activeCacheInfo.getSize();
		for ( int i = 0; i < numActiveEntries; ++i )
		{
			if ( m_activeCacheInfo[i].m_key == key )
			{
				return i + 1;
				break;
			}
		}
	}
	{
		const int numPreservedEntries = m_preservedCacheInfo.getSize();
		for ( int i = 0; i < numPreservedEntries; ++i )
		{
			if ( m_preservedCacheInfo[i].m_key == key )
			{
				return -(i + 1);
				break;
			}
		}
	}
	return 0;
}

template<typename KeyType>
hkInt32 hkObjectCache<KeyType>::getIndexByReference( hkReferencedObject* object ) const
{
	{
		const int numActiveEntries = m_activeCacheInfo.getSize();
		for ( int i = 0; i < numActiveEntries; ++i )
		{
			if ( m_activeCacheInfo[i].m_object == object )
			{
				return i + 1;
				break;
			}
		}
	}
	{
		const int numPreservedEntries = m_preservedCacheInfo.getSize();
		for ( int i = 0; i < numPreservedEntries; ++i )
		{
			if ( m_preservedCacheInfo[i].m_object == object )
			{
				return -(i + 1);
				break;
			}
		}
	}
	return 0;
}

template<typename KeyType>
hkObjectCache<KeyType>::~hkObjectCache()
{
	// Remove all references we hold to objects, active or preserved.
	{
		const int numActiveEntries = m_activeCacheInfo.getSize();
		for ( int i = 0; i < numActiveEntries; ++i )
		{
			m_activeCacheInfo[i].m_object->removeReference();
		}
	}
	{
		const int numPreservedEntries = m_preservedCacheInfo.getSize();
		for ( int i = 0; i < numPreservedEntries; ++i )
		{
			m_preservedCacheInfo[i].m_object->removeReference();
		}
	}
}

template<typename KeyType>
void hkObjectCache<KeyType>::purge()
{
	// Clear the preserved array, remembering to remove references.
	const int numPreservedEntries = m_preservedCacheInfo.getSize();
	for ( int i = 0; i < numPreservedEntries; ++i )
	{
		m_preservedCacheInfo[i].m_object->removeReference();
	}
	m_preservedCacheInfo.clearAndDeallocate();
}

template<typename KeyType>
void hkObjectCache<KeyType>::purgeEnoughFor( hkUint32 size )
{
	HK_ASSERT2( 0x3451a7ab, size <= m_maxSize, "Cannot purge that much from a cache this size." );
	// Free enough space.
	int lastIndexToKeep = 0;
	while ( m_maxSize - m_currentSize < size )
	{
		// Give up the reference this cache holds to the object.
		m_preservedCacheInfo[lastIndexToKeep].m_object->removeReference();
		m_currentSize -= m_preservedCacheInfo[lastIndexToKeep].m_size;
		++lastIndexToKeep;
		HK_ASSERT2( 0x563ad723, lastIndexToKeep <= m_preservedCacheInfo.getSize(), "Sum of preserved object sizes is too small" );
	}
	// Remove the unneeded entries from the array, preserving order
	if ( lastIndexToKeep > 0 )
	{
		m_preservedCacheInfo.removeAtAndCopy( 0, lastIndexToKeep );
	}
}

template<typename KeyType>
void hkObjectCache<KeyType>::setMaxSize( hkUint32 maxSize )
{
	int sizeDifference = m_maxSize - maxSize;
	if ( sizeDifference > 0 )
	{
		purgeEnoughFor( sizeDifference );
	}
	m_maxSize = maxSize;
}

template<typename KeyType>
void hkObjectCache<KeyType>::changeObject( KeyType key, hkReferencedObject* newObject, hkUint32 newSize )
{
	const int index = getIndexByKey( key );
	HK_ASSERT2( 0xb76e988a, index >= 0, "You cannot call changeObject for preserved objects. Use addObject instead." ); 
	HK_ASSERT2( 0xb76e988b, index > 0, "There is no active object for that key in the cache." ); 
	ActiveCacheInfo& oldInfo = m_activeCacheInfo[index - 1];
	newObject->addReference();
	oldInfo.m_object->removeReference();
	oldInfo.m_object = newObject;
	if ( newSize != -1 )
	{
		oldInfo.m_size = newSize;
	}
}

template<typename KeyType>
void hkObjectCache<KeyType>::changeObjectSize( KeyType key, hkUint32 newSize )
{
	const int index = getIndexByKey( key );
	HK_ASSERT2( 0xb76e988a, index >= 0, "You cannot call changeObject for preserved objects." ); 
	HK_ASSERT2( 0xb76e988b, index > 0, "There is no active object for that key in the cache." ); 
	ActiveCacheInfo& oldInfo = m_activeCacheInfo[index - 1];
	oldInfo.m_size = newSize;
}


extern const hkClass hkSerializedObjectCacheClass;
#include<Common/Serialize/Util/hkSerializeUtil.h>

template<typename KeyType>
inline void hkObjectCache<KeyType>::getAsPreserved( hkArray<PreservedCacheInfo>& arrayOut )
{
	getAsPreserved( arrayOut, m_maxSize );
}

template<typename KeyType>
void hkObjectCache<KeyType>::getAsPreserved( hkArray<PreservedCacheInfo>& arrayOut, hkUint32 maxSize )
{
	// We pick objects from the front of the active list (there's no perfect way to order them
	// since they are all currently active), and if there is enough room we add objects in the
	// preserved list. The latter are put at the front in their current order, since they
	// are older than the active objects.

	hkUint32 serializedSize = 0;
	int activeIndexBound = 0;
	const int numActiveObjects = m_activeCacheInfo.getSize();
	while ( ( activeIndexBound < numActiveObjects ) && ( m_maxSize - serializedSize >= m_activeCacheInfo[activeIndexBound].m_size ) )
	{
		serializedSize += m_activeCacheInfo[activeIndexBound].m_size;
		++activeIndexBound;
	}
	
	const int numPreservedObjects = m_preservedCacheInfo.getSize();
	for ( int i = 0; ( i < numPreservedObjects ) && ( m_maxSize - serializedSize >= m_preservedCacheInfo[i].m_size ); ++i )
	{
		arrayOut.expandOne() = m_preservedCacheInfo[i];
		serializedSize += m_preservedCacheInfo[i].m_size;
	}

	for ( int i = 0; i < activeIndexBound; ++i )
	{
		// Any additional information for PreservedCacheInfos not present ActiveCacheInfos should be added here.
		static_cast<CacheInfo&>( arrayOut.expandOne() ) = static_cast<CacheInfo&>( m_activeCacheInfo[i] );
	}
}

template<typename KeyType>
void hkObjectCache<KeyType>::addAsPreserved( hkArray<PreservedCacheInfo>& arrayIn )
{
	const int numNewInfos = arrayIn.getSize();
	// Gather the serialized objects to preserve. Do so in reverse order, since if there isn't
	// enough room for all of them, we'd prefer to keep the objects at the end.
	hkArray<int>::Temp newPreservedIndices;
	newPreservedIndices.reserveExactly( arrayIn.getSize() );
	hkUint32 sizeOfNewObjectsToPreserve = 0;
	for ( int i = numNewInfos - 1; i >= 0; --i )
	{
		PreservedCacheInfo& info = arrayIn[i];

		const int existingIndex = getIndexByKey( info.m_key );
		if ( existingIndex > 0 )
		{
			// There's an active object with the same hash, so ignore the new one.
			continue;
		}
		else if ( existingIndex < 0 )
		{
			// There's a preserved object with the same hash, so remove it.
			removePreservedObjectByIndex( existingIndex );
		}

		// Remember this index if there is room for this object.
		if ( m_maxSize - sizeOfNewObjectsToPreserve >= info.m_size )
		{
			newPreservedIndices.pushBack( i );
			sizeOfNewObjectsToPreserve += info.m_size;
		}
	}
	// Make room for the new objects.
	purgeEnoughFor( sizeOfNewObjectsToPreserve );
	// Copy them into the preserved array.
	m_currentSize += sizeOfNewObjectsToPreserve;
	const int numNewIndices = newPreservedIndices.getSize();
	PreservedCacheInfo* info = m_preservedCacheInfo.expandBy( numNewIndices );
	for ( int i = numNewIndices - 1; i >= 0; --i )
	{
		*info = arrayIn[newPreservedIndices[i]];
		info->m_object->addReference();
		++info;
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
