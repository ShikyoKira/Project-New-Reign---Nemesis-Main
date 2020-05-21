/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_STORAGE_STRINGMAP_H
#define HKBASE_STORAGE_STRINGMAP_H

#include <Common/Base/Container/StringMap/hkStringMap.h>

/// Like hkStringMap except that the keys are copied and managed internally.
template <typename V, typename Allocator=hkContainerHeapAllocator>
class hkStorageStringMap : public hkStringMap<V, Allocator>
{
	public:

		typedef typename hkStringMap<V>::Iterator Iterator;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MAP, hkStorageStringMap );

		~hkStorageStringMap()
		{
			clear();
		}

			///	Insert key with associated value val.
			/// If key already exists it is overwritten. The key string is not
			/// copied and must exist for the lifetime of the entry.
		HK_FORCE_INLINE const char* insert( const char* key, V val )
		{
			Iterator it = this->findKey(key);
			const char* ret;
			if( ! this->isValid(it) )
			{
				ret = hkString::strDup(key, Allocator().get(this) );
				hkStringMap<V,Allocator>::insert( ret, val );
			}
			else
			{
				ret = this->getKey(it);
				this->setValue(it, val);
			}
			return ret;
		}

		V getOrInsert(const char* key, V ifNotFound)
		{
			Iterator it = this->findKey(key);
			if( this->isValid(it) )
			{
				return this->getValue(it);
			}
			else
			{
				this->insert(key,ifNotFound);
				return ifNotFound;
			}
		}

			/// Remove pair at "it".
		void remove( Iterator it )
		{
			char* k = (char*)this->m_map.getKey(it);
			hkString::strFree( k, Allocator().get(this) );
			hkStringMap<V,Allocator>::remove( it );
		}

			/// If key present, remove it and return HK_SUCCESS. Otherwise return HK_FAILURE.
		hkResult remove( const char* key )
		{
			Iterator it = this->findKey(key);
			if( this->isValid(it) )
			{
				this->remove(it);
				return HK_SUCCESS;
			}
			return HK_FAILURE;
		}

			/// Remove all keys from the map.
		void clear()
		{
			for( Iterator it = hkStringMap<V,Allocator>::getIterator(); hkStringMap<V,Allocator>::isValid(it); it = hkStringMap<V,Allocator>::getNext(it) )
			{
				char* k = (char*)this->m_map.getKey(it);
				hkString::strFree( k, Allocator().get(this) );
			}
			hkStringMap<V,Allocator>::clear();
		}

			/// Swap all data with another map.
		void swap( hkStorageStringMap& other )
		{
			this->m_map.swap( other.m_map );
		}

		void merge( const hkStringMap<V,Allocator>& other )
		{
			typedef typename hkStringMap<V,Allocator>::Iterator OtherIterator;
			for( OtherIterator it = other.getIterator(); other.isValid(it); it = other.getNext(it) )
			{
				insert( other.getKey(it), other.getValue(it) );
			}
		}
};

#endif // HKBASE_STORAGE_STRINGMAP_H

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
