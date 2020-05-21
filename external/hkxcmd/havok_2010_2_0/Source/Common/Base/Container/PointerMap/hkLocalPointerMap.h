/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_LOCAL_POINTER_MAP_H
#define HK_LOCAL_POINTER_MAP_H

	/// An hkPointerMap that uses a stack allocation for its initial memory buffer.
template <typename K, typename V, typename Allocator=hkContainerHeapAllocator>
class hkLocalPointerMap : public hkPointerMap<K,V,Allocator>
{
	public:

			/// Construct with enough stack memory to store numKeys keys.
		HK_FORCE_INLINE hkLocalPointerMap( int numKeys )
		:	hkPointerMap<K,V,Allocator>::hkPointerMap(hkPointerMap<K,V,Allocator>::UNINITIALIZED)
		{
			m_numData = hkPointerMap<K,V,Allocator>::m_map.getSizeInBytesFor( numKeys );
			m_data = hkAllocateStack<char>( m_numData );
			hkPointerMap<K,V,Allocator>::m_map.init( m_data, m_numData );
		}

		HK_FORCE_INLINE ~hkLocalPointerMap()
		{
			hkDeallocateStack<char>( m_data, m_numData );
		}

			///	Calls hkPointerMap::insert() and asserts that a heap allocation did not occur.
		HK_FORCE_INLINE hkBool32 insert( K key, V val )
		{
			HK_ASSERT2( 0x6175b472, (hkPointerMap<K,V,Allocator>::getSize()) < (hkPointerMap<K,V,Allocator>::getCapacity()), "initial size too small, a heap allocation occured" );
			return hkPointerMap<K,V,Allocator>::insert( key, val );
		}

	private:

			// Memory allocated on the stack.
		hkPadSpu<char*> m_data;

			// The number of bytes allocated.
		int m_numData;
};

#endif

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
