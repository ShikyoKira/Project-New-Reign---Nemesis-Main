/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


/*   Hashing function : ( very simple for now )

     h(k) = k mod m   ( m = bucket size )

*/

inline hkBool hkaDefaultChunkCache::hashKey( struct hashKeyInfo& info ) 
{
	//
	// run the key through the hashing function ( each pool has its own modulus value == bucket size )
	//

	// which cache pool are we dealing with?
	for( hkUint32 i = 0; i < m_numberOfCachePools; i++ )
	{
		if( info.m_chunkSize < m_cachePools[i].m_chunkSize )
		{
			info.m_cachePool = i;
			break;
		}
	}

	if( info.m_cachePool == -1 )
	{
		// invalid cache pool

// add this request to the list of unavailable chunks
#if defined( HK_CACHE_STATS )

		// check if the array already has this chunk
		for( hkInt32 i = 0; i < m_unavailableChunks.getSize(); i++ )
		{
			if( m_unavailableChunks[i].m_chunkSize == info.m_chunkSize )
			{
				// update frequency
				m_unavailableChunks[i].m_frequency++;
				return false;
			}

		}

		// pushback a new unavailable chunk
		struct unavailableChunk uChunk;

		uChunk.m_chunkSize = info.m_chunkSize;
		uChunk.m_frequency = 1;

		m_unavailableChunks.pushBack( uChunk );

#endif

		HK_WARN_ONCE( 0x1cc3730c, "No cache pool enabled to deal with chunk size!" );

		// failure
		return false;
	}

	// hash the key and store result
	info.m_bucket = ( info.m_key % m_cachePools[ info.m_cachePool ].m_buckets );

	// success
	return true;

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
