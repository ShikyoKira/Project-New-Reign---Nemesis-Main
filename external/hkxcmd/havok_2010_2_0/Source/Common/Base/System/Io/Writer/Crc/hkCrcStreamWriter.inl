/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template<typename T, T poly>
hkCrcStreamWriter<T, poly>::hkCrcStreamWriter(T startCrc)
: m_crc(startCrc ^ T(-1))
{
}

template<typename T, T poly>
T hkCrcStreamWriter<T, poly>::calcPermute(T p)
{
	for( int i = 0; i < 8; ++i )
	{
		if( p & 1 )
		{
			p = poly ^ (p >> 1);
		}
		else
		{
			p = p >> 1;
		}
	}
	return p;
}

template<typename T, T poly>
int hkCrcStreamWriter<T, poly>::write(const void* ptr, int nbytes)
{
	const hkUint8* buf = static_cast<const hkUint8*>(ptr);
	T crc = m_crc;
	for( int n = 0; n < nbytes; ++n)
	{
		T permute = calcPermute( (crc ^ buf[n]) & 0xff );
		crc = permute ^ (crc >> 8);
	}
	m_crc = crc;
	return nbytes;
}

template<typename T, T poly>
T hkCrcStreamWriter<T, poly>::getCrc() const
{
	return m_crc ^ T(-1);
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
