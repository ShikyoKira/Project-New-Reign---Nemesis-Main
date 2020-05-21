/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSERIALIZE_RoundTrip_H
#define HKSERIALIZE_RoundTrip_H

extern const hkClass hkRoundTripClass;

class hkRoundTrip
{
	public:

		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO,hkRoundTrip);

		enum FlagBits
		{
			BIT_X = 1,
			BIT_Y = 2,
			BIT_Z = 4,
			BIT_XZ = 5
		};

		hkRoundTrip() :
			m_flags0(0),
			m_flags1(BIT_X),
			m_flags2(BIT_XZ|BIT_Y),
			m_flags3(BIT_Z |0xf0)
		{
		}

		hkFlags<FlagBits, hkInt32> m_flags0;
		hkFlags<FlagBits, hkInt32> m_flags1;
		hkFlags<FlagBits, hkInt32> m_flags2;
		hkFlags<FlagBits, hkInt32> m_flags3;
};
#endif // HKSERIALIZE_RoundTrip_H

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
