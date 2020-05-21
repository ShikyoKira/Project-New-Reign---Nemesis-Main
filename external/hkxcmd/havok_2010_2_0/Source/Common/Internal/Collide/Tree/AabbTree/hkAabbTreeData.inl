/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/Math/Vector/hkVector4Util.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabbUtil.h>

//=====================
//	hkAabbTreeAabb16
HK_FORCE_INLINE int hkAabbTreeData::hkAabbTreeAabb16::Metric::select(const hkAabbTreeAabb16& a, const hkAabbTreeAabb16& b) const
{
	hkVector4 centerA; a.center( centerA );
	hkVector4 centerB; b.center( centerB );
	centerA.sub4( m_center );
	centerB.sub4( m_center );
	centerA.mul4( centerA );
	centerB.mul4( centerB );
	centerB.sub4( centerA );
#if defined(HK_PLATFORM_SPU) || defined(HK_PLATFORM_RVL) || defined(HK_PLATFORM_PSP) || defined(HK_PLATFORM_MAC386) || (defined(HK_PLATFORM_WIN32) && (HK_CONFIG_SIMD == HK_CONFIG_SIMD_DISABLED))
	int index = (hkMath::isNegative( centerB.horizontalAdd3() )) ? 0 : 1; 
#else
	int index = hkMath::isNegative( centerB.horizontalAdd3() );
#endif
	HK_ASSERT( 0xf0324567, index ==0 || index == 1);
	return index;
}

HK_FORCE_INLINE hkAabbTreeData::hkAabbTreeAabb16::Metric hkAabbTreeData::hkAabbTreeAabb16::getMetric() const
{
	hkAabbTreeData::hkAabbTreeAabb16::Metric metric;
	center( metric.m_center );
	return metric;
}

HK_FORCE_INLINE void hkAabbTreeData::hkAabbTreeAabb16::center(hkVector4& centerOut) const
{
	centerOut.set( m_max[0], m_max[1], m_max[2] );
	hkVector4 myMin;
	myMin.set( m_min[0], m_min[1], m_min[2] );
	centerOut.add4( myMin );
	centerOut.mul4( hkSimdReal(hkQuadRealHalf) );
}

HK_FORCE_INLINE void hkAabbTreeData::hkAabbTreeAabb16::combine(const hkAabbTreeAabb16& a, const hkAabbTreeAabb16& b, hkAabbTreeAabb16& out)
{
	for(int i=0; i<3; i++)
	{
		out.m_min[i] = hkMath::min2( a.m_min[i], b.m_min[i] );
		out.m_max[i] = hkMath::max2( a.m_max[i], b.m_max[i] );
	}
}

HK_FORCE_INLINE hkBool32 hkAabbTreeData::hkAabbTreeAabb16::equals(const hkAabbTreeAabb16& aabb) const
{
	for(int i=0; i<3; i++)
	{
		if( aabb.m_min[i] != m_min[i] || aabb.m_max[i] != m_max[i] )
		{
			return false;
		}
	}
	return true;
}

HK_FORCE_INLINE hkBool hkAabbTreeData::hkAabbTreeAabb16::contains(const hkAabbTreeAabb16& other) const
{
	bool outside = false;
	for(int i=0; i<3; i++)
	{
		outside |= (m_max[i] < other.m_max[i] || other.m_min[i] < m_min[i]);
	}
	return !outside;
}

HK_FORCE_INLINE void hkAabbTreeData::hkAabbTreeAabb16::includeAabb( const hkAabbTreeAabb16& aabb)
{
	combine( *this, aabb, *this );
}

HK_FORCE_INLINE hkBool hkAabbTreeData::hkAabbTreeAabb16::isValid() const
{
	bool valid = true;
	for(int i=0; i<3; i++)
	{
		valid &= (m_max[i] >= m_min[i]);
	}
	return (hkBool)valid;
}

HK_FORCE_INLINE void hkAabbTreeData::hkAabbTreeAabb16::setEmpty()
{
	for(int i=0; i<3; i++)
	{
		m_max[i] = 0;
		m_min[i] = 0xffff;
	}
}

HK_FORCE_INLINE hkBool32 hkAabbTreeData::hkAabbTreeAabb16::overlaps( const hkAabbTreeAabb16& testAabb ) const
{
	hkBool32 xCmp = (m_min[0] <= testAabb.m_max[0]) && (testAabb.m_min[0] <= m_max[0]);
	hkBool32 yCmp = (m_min[1] <= testAabb.m_max[1]) && (testAabb.m_min[1] <= m_max[1]);
	hkBool32 zCmp = (m_min[2] <= testAabb.m_max[2]) && (testAabb.m_min[2] <= m_max[2]);

	return xCmp && yCmp && zCmp;
}

hkBool32 hkAabbTreeData::hkAabbTreeAabb16::disjoint(const hkAabbTreeAabb16& testAabb) const
{
	int x0 = int(m_max[0]) - int(testAabb.m_min[0]);
	int x1 = int(testAabb.m_max[0]) - int(m_min[0]);

	int y0 = int(m_max[1]) - int(testAabb.m_min[1]);
	int z0 = int(m_max[2]) - int(testAabb.m_min[2]);

	int y1 = int(testAabb.m_max[1]) - int(m_min[1]);
	int z1 = int(testAabb.m_max[2]) - int(m_min[2]);

	x0 |= y0;
	x1 |= y1;
	x0 |= z0;
	x1 |= z1;

	x0 |= x1;

	hkBool32 ovl = (~x0) & 0x80000000;

	return ovl;

}

HK_FORCE_INLINE hkReal hkAabbTreeData::hkAabbTreeAabb16::distanceSquared( const hkVector4& point ) const
{
	HK_ASSERT(0x118c2eb0, 1);
	return 0;
}

HK_FORCE_INLINE hkBool32 hkAabbTreeData::hkAabbTreeAabb16::overlaps( const hkAabb& testAabb ) const
{
	hkAabb thisAabb;
	thisAabb.m_max.set( m_max[0], m_max[1], m_max[2] );
	thisAabb.m_min.set( m_min[0], m_min[1], m_min[2] );

	return thisAabb.overlaps( testAabb );
}



HK_FORCE_INLINE
void hkAabbTreeData::hkAabbTreeAabb16::convertAabbToTreeSpace(const hkAabb&volume, const hkVector4& bitOffsetLow, const hkVector4& bitOffsetHigh, const hkVector4& bitScale, hkAabbTreeData::hkAabbTreeAabb16& volumeOut)
{
	hkAabbUint32 qAabb;
	hkVector4Util::convertAabbToUint32( volume, bitOffsetLow, bitOffsetHigh, bitScale, qAabb );
	convertAabbToTreeSpace( qAabb, bitOffsetLow, bitOffsetHigh, bitScale, volumeOut );
}

HK_FORCE_INLINE
void hkAabbTreeData::hkAabbTreeAabb16::convertAabbToTreeSpace( const hkAabbUint32& volumeIn, const hkVector4& bitOffsetLow, const hkVector4& bitOffsetHigh, const hkVector4& bitScale,   hkAabbTreeData::hkAabbTreeAabb16& volumeOut)
{
	for(int i=0; i<3; i++)
	{
		volumeOut.m_max[i] = (hkUint16)((volumeIn.m_max[i]) >> 15);
		volumeOut.m_min[i] = (hkUint16)((volumeIn.m_min[i]) >> 15);
	}
}

HK_FORCE_INLINE
void hkAabbTreeData::hkAabbTreeAabb16::restoreAabb(const hkAabbTreeAabb16& in, const hkVector4& bitScaleInv, const hkVector4& bitOffsetLow, hkAabb& out)
{
	out.m_max.set( (hkReal)(in.m_max[0] << 15), (hkReal)(in.m_max[1] << 15), (hkReal)(in.m_max[2] << 15) );
	out.m_min.set( (hkReal)(in.m_min[0] << 15), (hkReal)(in.m_min[1] << 15),(hkReal)(in.m_min[2] << 15) );

	out.m_min.mul4( bitScaleInv );
	out.m_max.mul4( bitScaleInv );
	out.m_min.sub4( bitOffsetLow );
	out.m_max.sub4( bitOffsetLow );
}



//=====================
//	hkAabbTreeAabb

HK_FORCE_INLINE int hkAabbTreeData::hkAabbTreeAabb::Metric::select(const hkAabbTreeAabb& a, const hkAabbTreeAabb& b) const
{
	hkVector4 centerA; centerA.setAdd4( a.m_min, a.m_max );
	hkVector4 centerB; centerB.setAdd4( b.m_min, b.m_max );
	centerA.sub4( m_center );
	centerB.sub4( m_center );
	centerA.mul4( centerA );
	centerB.mul4( centerB );
	centerB.sub4( centerA );
	int index = (hkMath::isNegative( centerB.horizontalAdd3() )) ? 0 : 1;
	HK_ASSERT( 0xf0324567, index ==0 || index == 1);
	return index;
}

HK_FORCE_INLINE hkAabbTreeData::hkAabbTreeAabb::Metric hkAabbTreeData::hkAabbTreeAabb::getMetric() const
{
	hkAabbTreeData::hkAabbTreeAabb::Metric metric;
	metric.m_center.setAdd4(m_min,m_max);
	return(metric);
}

HK_FORCE_INLINE void hkAabbTreeData::hkAabbTreeAabb::center( hkVector4& centerOut ) const
{
	centerOut.setAdd4( m_min, m_max );
	centerOut.mul4( hkSimdReal(hkQuadRealHalf) );
}

HK_FORCE_INLINE void hkAabbTreeData::hkAabbTreeAabb::combine( const hkAabbTreeAabb& a, const hkAabbTreeAabb& b, hkAabbTreeAabb& out)
{
	out.m_min.setMin4( a.m_min, b.m_min );
	out.m_max.setMax4( a.m_max, b.m_max );
}

HK_FORCE_INLINE hkBool32 hkAabbTreeData::hkAabbTreeAabb::overlaps(const Ray& r, const hkVector4& earlyOutHitFraction ) const
{
	hkVector4 dNearTemp;
	hkVector4 dFarTemp;	

	dNearTemp.setSub4(m_min, r.m_start );
	dFarTemp.setSub4( m_max, r.m_start );

	dNearTemp.mul4(r.m_invDir);
	dFarTemp.mul4(r.m_invDir);

	hkVector4 dNear; dNear.setMin4( dNearTemp, dFarTemp );
	hkVector4 dFar;  dFar.setMax4(  dNearTemp, dFarTemp );

	hkVector4 dNearY; dNearY.setBroadcast( dNear, 1 );
	hkVector4 dNearZ; dNearZ.setBroadcast( dNear, 2 );
	hkVector4 dFarY; dFarY.setBroadcast( dFar, 1 );
	hkVector4 dFarZ; dFarZ.setBroadcast( dFar, 2 );

	hkVector4 nearYZ; nearYZ.setMax4( dNearY, dNearZ );
	hkVector4 farYZ;   farYZ.setMin4( dFarY, dFarZ );
	hkVector4 nearX; nearX.setMax4( dNear, hkVector4::getZero() );
	hkVector4 farX; farX.setMin4( dFar,  earlyOutHitFraction );

	nearX.setMax4( nearX, nearYZ );
	farX.setMin4( farX, farYZ );

	hkVector4Comparison nearLtFar = nearX.compareLessThan4(farX);
	return nearLtFar.anyIsSet(hkVector4Comparison::MASK_X);
}

HK_FORCE_INLINE hkBool32 hkAabbTreeData::hkAabbTreeAabb::overlaps(const RayBundle& ray, const hkVector4& ealyOutFractions, hkVector4Comparison& hitOut) const
{
	hkVector4 max[3]; 
	max[0].setBroadcast( m_max, 0 );
	max[1].setBroadcast( m_max, 1 );
	max[2].setBroadcast( m_max, 2 );

	hkVector4 min[3]; 
	min[0].setBroadcast( m_min, 0 );
	min[1].setBroadcast( m_min, 1 );
	min[2].setBroadcast( m_min, 2 );

	Bundle dNear;
	Bundle dFar;

	for(int i=0; i<3; i++)
	{
		hkVector4 dNearTmp;
		hkVector4 dFarTmp;
		dNearTmp.setSub4( min[i], ray.m_from.m_data[i]);
		dFarTmp.setSub4( max[i], ray.m_from.m_data[i]);
		dNearTmp.mul4( ray.m_invDir.m_data[i] );
		dFarTmp.mul4( ray.m_invDir.m_data[i] );

		dNear.m_data[i].setMin4( dNearTmp, dFarTmp );
		dFar.m_data[i].setMax4( dNearTmp, dFarTmp );
	}

	hkVector4 maxNear;
	hkVector4 minFar;

	maxNear.setMax4( dNear.m_data[0], dNear.m_data[1] );
	maxNear.setMax4( maxNear, dNear.m_data[2] );
	maxNear.setMax4( maxNear, hkVector4::getZero() );

	minFar.setMin4( dFar.m_data[0], dFar.m_data[1] );
	minFar.setMin4( minFar, dFar.m_data[2] );
	minFar.setMin4( minFar, ealyOutFractions );

	hitOut.set( ray.m_mask );
	hitOut.setAnd( maxNear.compareLessThan4( minFar ), hitOut );

	return hitOut.anyIsSet(ray.m_mask);
}

HK_FORCE_INLINE hkBool32 hkAabbTreeData::hkAabbTreeAabb::overlaps(const hkAabb& aabb) const
{
	return hkAabb::overlaps( aabb );
}

HK_FORCE_INLINE void hkAabbTreeData::hkAabbTreeAabb::expand( hkVector4Parameter velocity, hkReal margin )
{
	hkVector4 zero; zero.setZero4();
	hkVector4 marginV; marginV.setAll( margin );
	hkVector4 velPos; velPos.setMax4( velocity, zero );
	hkVector4 velNeg; velNeg.setMin4( velocity, zero );

	hkVector4 inc; inc.setAdd4( velPos, marginV);
	hkVector4 dec; dec.setSub4( velNeg, marginV);
	m_max.add4( inc );
	m_min.add4( dec );
}

HK_FORCE_INLINE hkReal hkAabbTreeData::hkAabbTreeAabb::distanceSquared(const hkVector4& point) const
{
	return hkAabbUtil::distanceSquared( *this, point );
}

HK_FORCE_INLINE
void hkAabbTreeData::hkAabbTreeAabb::convertAabbToTreeSpace(const hkAabb&volume, const hkVector4& bitOffsetLow, const hkVector4& bitOffsetHigh, const hkVector4& bitScale, hkAabbTreeAabb& volumeOut)
{
	volumeOut.m_max = volume.m_max;
	volumeOut.m_min = volume.m_min;
}

HK_FORCE_INLINE
void hkAabbTreeData::hkAabbTreeAabb::convertAabbToTreeSpace( const hkAabbUint32& volumeIn, const hkVector4& bitOffsetLow, const hkVector4& bitOffsetHigh, const hkVector4& bitScale,   hkAabbTreeAabb& volumeOut)
{
	hkVector4Util::convertAabbFromUint32( volumeIn, bitOffsetLow, bitScale, volumeOut );
}

HK_FORCE_INLINE
void hkAabbTreeData::hkAabbTreeAabb::restoreAabb(const hkAabbTreeAabb& in, const hkVector4& bitScaleInv, const hkVector4& bitOffsetLow, hkAabb& out)
{
	out = in;
}


//=====================
//	others
HK_FORCE_INLINE void hkAabbTreeData::Ray::setRay(const hkVector4& start, const hkVector4& end)
{
	static const hkQuadReal QuadRealMax_Const = HK_QUADREAL_CONSTANT(HK_REAL_MAX,HK_REAL_MAX,HK_REAL_MAX,HK_REAL_MAX);
	m_start = start;
	hkVector4 delta; delta.setSub4( end, start );
	hkVector4Comparison isZero = delta.compareEqual4( hkVector4::getZero() );
	hkVector4 maskedDelta; maskedDelta.select32(delta, hkVector4::getConstant(HK_QUADREAL_1), isZero);

	hkVector4 invDelta; invDelta.setReciprocal3( maskedDelta );
	m_invDir.select32( invDelta, (const hkVector4&)QuadRealMax_Const, isZero );	
}

HK_FORCE_INLINE hkAabbTreeData::RayBundle::RayBundle( const Bundle& from, const Bundle& to, hkVector4Comparison::Mask mask )
{
	hkVector4 realMax4; realMax4 = hkVector4::getConstant<HK_QUADREAL_MAX>();

	m_mask = mask;

	m_from = from;
	Bundle delta;
	for(int i=0; i<3; i++)
	{
		delta.m_data[i].setSub4( to.m_data[i], m_from.m_data[i] );
		hkVector4Comparison isZero = delta.m_data[i].compareEqual4( hkVector4::getZero() );
		hkVector4 maskedDelta; maskedDelta.select32( delta.m_data[i], hkVector4::getConstant(HK_QUADREAL_1), isZero );
		hkVector4 invDelta;
		invDelta.setReciprocal4( maskedDelta );
		m_invDir.m_data[i].select32( invDelta, realMax4, isZero );
	}
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
