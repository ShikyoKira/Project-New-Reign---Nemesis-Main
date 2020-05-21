/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE hkIstream& hkIstream::operator>> (signed char& c)
{
	return (*this) >> reinterpret_cast<char&>(c);
}

HK_FORCE_INLINE hkIstream& hkIstream::operator>> (unsigned char& c)
{
	return (*this) >> reinterpret_cast<char&>(c);
}

HK_FORCE_INLINE hkIstream& hkIstream::get( char& c )
{
	return (*this) >> c;
}

HK_FORCE_INLINE hkStreamReader* hkIstream::getStreamReader()
{
	return m_streamReader;
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
