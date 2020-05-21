/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MATH_AABB_HALF_H
#define HK_MATH_AABB_HALF_H

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Common/Base/Math/Vector/hkIntVector.h>

struct hkAabbHalf
{
	HK_DECLARE_REFLECTION();
	HK_FORCE_INLINE	void	pack(const hkAabb& aabb)
	{
		const hkUint32		extras = *(const hkUint32*)m_extras;
		hkVector4Comparison	maskMin = aabb.m_min.compareLessThanZero4();
		hkVector4Comparison	maskMax = aabb.m_max.compareLessThanZero4();
		hkIntVector			iminD; iminD.setAnd(	reinterpret_cast<const hkIntVector&>(aabb.m_min) , reinterpret_cast<const hkIntVector&>(cstRoundingD));
		hkIntVector			iminU; iminU.setAddU32(	reinterpret_cast<const hkIntVector&>(aabb.m_min) , reinterpret_cast<const hkIntVector&>(cstRoundingU));
		hkIntVector			imaxD; imaxD.setAnd(	reinterpret_cast<const hkIntVector&>(aabb.m_max) , reinterpret_cast<const hkIntVector&>(cstRoundingD));
		hkIntVector			imaxU; imaxU.setAddU32(	reinterpret_cast<const hkIntVector&>(aabb.m_max) , reinterpret_cast<const hkIntVector&>(cstRoundingU));
		hkIntVector			imin; imin.setSelect(maskMin,iminU,iminD);
		hkIntVector			imax; imax.setSelect(maskMax,imaxD,imaxU);
		hkIntVector			interleaved;
		imin.setShiftRight32<16>(imin);
		imax.setAnd(imax,reinterpret_cast<const hkIntVector&>(cstRoundingD));
		interleaved.setOr(imin,imax);
		interleaved.storeUnaligned4(m_data);
		*(hkUint32*)m_extras = extras;
	}

	HK_FORCE_INLINE void	unpackUnaligned(hkAabb& aabb) const
	{
		hkIntVector	interleaved; interleaved.loadUnaligned4((const hkUint32*)m_data);
		hkIntVector zero; zero.setZero();
		hkIntVector	imin; imin.setShiftLeft32<16>(interleaved); imin.setAnd(imin,reinterpret_cast<const hkIntVector&>(cstRoundingD));
		hkIntVector	imax; imax.setAnd(interleaved,reinterpret_cast<const hkIntVector&>(cstRoundingD));
		aabb.m_max = reinterpret_cast<const hkVector4&>(imax);
		aabb.m_min = reinterpret_cast<const hkVector4&>(imin);
	}

	HK_FORCE_INLINE void	unpackAligned(hkAabb& aabb) const
	{
		hkIntVector	interleaved; interleaved.loadAligned((const hkUint32*)m_data);
		hkIntVector zero; zero.setZero();
		hkIntVector	imin; imin.setShiftLeft32<16>(interleaved); imin.setAnd(imin,reinterpret_cast<const hkIntVector&>(cstRoundingD));
		hkIntVector	imax; imax.setAnd(interleaved,reinterpret_cast<const hkIntVector&>(cstRoundingD));
		aabb.m_max = reinterpret_cast<const hkVector4&>(imax);
		aabb.m_min = reinterpret_cast<const hkVector4&>(imin);
	}

	hkUint16	m_data[6];		///< Packed bounding box min/max.
	hkUint16	m_extras[2];	///< Extras padding data, kept intact during packing operation

	static HK_ALIGN16(const hkUint32 cstRoundingU[4]);
	static HK_ALIGN16(const hkUint32 cstRoundingD[4]);
};

#endif // HK_MATH_AABB_HALF_H

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
