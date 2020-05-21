/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKANIMATION_ANIMATION_QUANTIZED_HKQUANTIZEDQUATERNION2_XML_H
#define HKANIMATION_ANIMATION_QUANTIZED_HKQUANTIZEDQUATERNION2_XML_H

#include <Animation/Animation/Animation/hkaAnimation.h>

class hkaQuantizedQuaternion
{
public:

	static void HK_CALL packQuaternions48( hkUint16 dst[ 12 ], const hkQuaternion& src0, const hkQuaternion& src1, const hkQuaternion& src2, const hkQuaternion& src3 );

	static void HK_CALL unpackQuaternions48( hkQuaternion& dst0, hkQuaternion& dst1, hkQuaternion& dst2, hkQuaternion& dst3, const hkUint16 qq[ 12 ] );
};

#endif // HKANIMATION_ANIMATION_QUANTIZED_HKQUANTIZEDQUATERNION2_XML_H

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
