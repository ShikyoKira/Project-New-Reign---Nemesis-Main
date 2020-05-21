/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline void hkp26Dop::expand(const hkReal tolerance)
{
	hkReal* v = &m_lx;
	for (int i=0; i < 26;)
	{
		v[i++] -= tolerance; // sub from low
		v[i++] += tolerance; // add to high
	}
}

inline hkReal hkp26Dop::getLow(int i ) const
{
	return (&m_lx)[i*2];
}

inline hkReal hkp26Dop::getHigh(int i ) const
{
	return (&m_hx)[i*2];
}

inline void hkp26Dop::setLow(int i, hkReal l)
{
	(&m_lx)[i<<1] = l;
}

inline void hkp26Dop::setHigh(int i, hkReal h)
{
	(&m_hx)[i<<1] = h;
}

inline hkReal hkp26Dop::getLowDistance(int i ) const
{
	if (i < 3)
	{
		return (&m_lx)[i<<1];
	}
	else if (i < 9)
	{
		return (&m_lx)[i<<1] / ROOT2;
	}
	else return (&m_lx)[i<<1] / ROOT3;

}

inline hkReal hkp26Dop::getHighDistance(int i ) const
{
	if (i < 3)
	{
		return (&m_hx)[i<<1];
	}
	else if (i < 9)
	{
		return (&m_hx)[i<<1] / ROOT2;
	}
	else return (&m_hx)[i<<1] / ROOT3;

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
