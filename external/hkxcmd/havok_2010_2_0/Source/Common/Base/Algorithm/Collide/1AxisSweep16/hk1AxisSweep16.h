/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKNP_1AXIS_SWEEP_H
#define HKNP_1AXIS_SWEEP_H

#include <Common/Base/Types/Geometry/Aabb16/hkAabb16.h>

template <typename T>
class hk1AxisSweep16
{
	public:
		struct KeyPair
		{
			T m_keyA;
			T m_keyB;
		};

		static int HK_CALL collide( const hkAabb16* pa, int numA, const hkAabb16* pb, int numB, 
			KeyPair* HK_RESTRICT pairsOut, int maxNumPairs, hkPadSpu<int>& numPairsSkipped );

		static int HK_CALL collide( const hkAabb16* pa, int numA, 
			KeyPair* HK_RESTRICT pairsOut, int maxNumPairs, hkPadSpu<int>& numPairsSkippedOut );

		static void HK_CALL sortAabbs(hkAabb16* aabbs, int size);
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
