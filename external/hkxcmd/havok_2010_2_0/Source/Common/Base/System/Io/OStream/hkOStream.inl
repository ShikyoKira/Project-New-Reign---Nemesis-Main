/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE hkStreamWriter*	hkOstream::getStreamWriter()
{
	return m_writer;
}

HK_FORCE_INLINE hkOstream& hkOstream::operator<< (signed char c)
{
	return (*this) << static_cast<char>(c);
}

HK_FORCE_INLINE hkOstream& hkOstream::operator<< (unsigned char c)
{
	return (*this) << static_cast<char>(c);
}

HK_FORCE_INLINE hkOstream& hkOstream::operator<< (const signed char* s)
{
	return (*this) << reinterpret_cast<const char*>(s);
}

HK_FORCE_INLINE hkOstream& hkOstream::operator<< (const unsigned char* s)
{
	return (*this) << reinterpret_cast<const char*>(s);
}

HK_FORCE_INLINE hkOstream& hkOstream::operator<< (double d)
{
	return (*this) << static_cast<float>(d);
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
