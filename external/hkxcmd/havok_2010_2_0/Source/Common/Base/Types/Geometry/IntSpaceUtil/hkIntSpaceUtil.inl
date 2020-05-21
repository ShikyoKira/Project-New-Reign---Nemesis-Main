/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/Math/Vector/hkIntVector.h>


void hkIntSpaceUtil::convertWithOffsetLow( const hkVector4& vIn, hkUint16 vOut[3] ) const
{
	hkVector4 v;
	v.setAdd4( m_bitOffsetLow, vIn );
	v.mul4( m_bitScale );

	hkIntVector imi; imi.setConvertF32toS32( v );

	hkUint16 vi[8];
	((hkIntVector&)vi).setConvertU32ToU16( imi, imi );

	vOut[0] = vi[0];
	vOut[1] = vi[1];
	vOut[2] = vi[2];
}

void hkIntSpaceUtil::convertAabb(const hkAabb& aabbF, hkAabb16& aabbOut)const
{
	hkVector4 minVal; minVal.setZero4();
	hkVector4 maxVal; maxVal.getQuad() = s_aabb16Max;

	hkVector4 mi;  mi.setAdd4( m_bitOffsetLow,  aabbF.m_min );
	hkVector4 ma;  ma.setAdd4( m_bitOffsetHigh, aabbF.m_max );

	mi.mul4( m_bitScale );
	ma.mul4( m_bitScale );

	mi.setMax4( minVal, mi );
	ma.setMax4( minVal, ma );
	mi.setMin4( maxVal, mi );
	ma.setMin4( maxVal, ma );

	hkIntVector imi; imi.setConvertF32toS32( mi );
	hkIntVector ima; ima.setConvertF32toS32( ma );

	hkUint16 key0 = aabbOut.m_key;
	hkUint16 key1 = aabbOut.m_key1;
	((hkIntVector&)aabbOut).setConvertU32ToU16( imi, ima );

	aabbOut.m_key = key0;
	aabbOut.m_key1 = key1;
}


void hkIntSpaceUtil::restoreAabb(const hkAabb16& aabbI, hkAabb& aabbOut)const
{
	hkIntVector iZero; iZero.setZero();
	hkIntVector intAabb; intAabb.loadAligned( (const hkUint32*)&aabbI);

	hkIntVector iMi; iMi.setCombineHead16To32( iZero, intAabb );
	hkIntVector iMa; iMa.setCombineTail16To32( iZero, intAabb );

	hkVector4 mi; iMi.convertS32ToF32( mi );
	hkVector4 ma; iMa.convertS32ToF32( ma );

	mi.mul4( m_bitScaleInv );
	ma.mul4( m_bitScaleInv );
	mi.sub4( m_bitOffsetLow );
	ma.sub4( m_bitOffsetHigh );

	aabbOut.m_min = mi;
	aabbOut.m_max = ma;

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
