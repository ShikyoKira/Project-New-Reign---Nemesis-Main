/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_AABB_TREE_H
#define HK_AABB_TREE_H

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

class hkJobQueue;
class hkJobThreadPool;


class hkAabbTreeData
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkAabbTreeData);
	struct Ray
	{
		HK_FORCE_INLINE void setRay( const hkVector4& start, const hkVector4& end );

		hkVector4 m_start;
		hkVector4 m_invDir;
	};

	struct Bundle
	{
		hkVector4 m_data[3]; // 0:X, 1:Y, 2:z
	};

	struct SetRays
	{
		SetRays(){ m_numActiveRays = 4;}
		hkVector4 m_from[4];
		hkVector4 m_to[4];
		int m_numActiveRays;
	};

	struct RayBundle
	{
		HK_FORCE_INLINE RayBundle(){}
		HK_FORCE_INLINE RayBundle( const Bundle& from, const Bundle& to, hkVector4Comparison::Mask mask = hkVector4Comparison::MASK_XYZW );

		Bundle m_from;
		Bundle m_invDir;

		hkVector4Comparison::Mask m_mask;
	};

	struct CastVolumeInput
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, CastVolumeInput );

		hkVector4 m_from;
		hkVector4 m_to;
		hkVector4 m_halfExtents;
	};


	struct hkAabbTreeAabb16
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkAabbTreeData::hkAabbTreeAabb16);
		struct Metric
		{
			//+hk.MemoryTracker(ignore=True)
			hkVector4	m_center;
			/// Find the best matching AABB
			HK_FORCE_INLINE int select(const hkAabbTreeAabb16& a, const hkAabbTreeAabb16& b) const;
		};
		typedef Metric METRIC;
		HK_FORCE_INLINE Metric getMetric() const;

		HK_FORCE_INLINE hkAabbTreeAabb16(){}
		HK_FORCE_INLINE void center( hkVector4& centerOut ) const;
		HK_FORCE_INLINE static void combine( const hkAabbTreeAabb16& a, const hkAabbTreeAabb16& b, hkAabbTreeAabb16& out);
		HK_FORCE_INLINE hkBool32 equals(const hkAabbTreeAabb16& aabb) const;
		HK_FORCE_INLINE hkBool contains(const hkAabbTreeAabb16& other) const;
		HK_FORCE_INLINE void includeAabb( const hkAabbTreeAabb16& aabb);
		HK_FORCE_INLINE hkBool isValid() const;
		HK_FORCE_INLINE void setEmpty();
		HK_FORCE_INLINE hkBool32 overlaps( const hkAabbTreeAabb16& testAabb) const;
		HK_FORCE_INLINE hkBool32 disjoint( const hkAabbTreeAabb16& testAabb) const;
		HK_FORCE_INLINE hkReal distanceSquared(const hkVector4& point) const;

		HK_FORCE_INLINE static void HK_CALL convertAabbToTreeSpace(const hkAabb& volume, const hkVector4& bitOffsetLow, const hkVector4& bitOffsetHigh, const hkVector4& bitScale, hkAabbTreeAabb16& volumeOut);
		HK_FORCE_INLINE static void HK_CALL convertAabbToTreeSpace( const hkAabbUint32& volumeIn, const hkVector4& bitOffsetLow, const hkVector4& bitOffsetHigh, const hkVector4& bitScale,  hkAabbTreeAabb16& volumeOut);
		HK_FORCE_INLINE static void HK_CALL restoreAabb(const hkAabbTreeAabb16& in, const hkVector4& bitScaleInv, const hkVector4& bitOffsetLow, hkAabb& out);


		//=================
		//	temporary function for transition of quantized AABB tree. they will be deleted
		HK_FORCE_INLINE hkBool32 overlaps( const hkAabb& testAabb ) const;


		hkUint16 m_max[3];
		hkUint16 m_min[3];
	};

	struct hkAabbTreeAabb : public hkAabb
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkAabbTreeData::hkAabbTreeAabb);
		struct Metric
		{
			//+hk.MemoryTracker(ignore=True)
			hkVector4	m_center;
			/// Find the best matching AABB.
			HK_FORCE_INLINE int select(const hkAabbTreeAabb& a, const hkAabbTreeAabb& b) const;
		};
		typedef Metric METRIC;
		HK_FORCE_INLINE Metric getMetric() const;
		HK_FORCE_INLINE void center( hkVector4& centerOut ) const;
		HK_FORCE_INLINE static void combine( const hkAabbTreeAabb& a, const hkAabbTreeAabb& b, hkAabbTreeAabb& out);
		HK_FORCE_INLINE hkBool32 overlaps(const Ray& r, const hkVector4& earlyOutHitFraction ) const;
		HK_FORCE_INLINE hkBool32 overlaps(const RayBundle& ray, const hkVector4& ealyOutFractions, hkVector4Comparison& hitOut) const;
		HK_FORCE_INLINE hkBool32 overlaps(const hkAabb& aabb) const;
		HK_FORCE_INLINE void expand( hkVector4Parameter velocity, hkReal margin );
		HK_FORCE_INLINE hkReal distanceSquared(const hkVector4& point) const;

		HK_FORCE_INLINE static void HK_CALL convertAabbToTreeSpace(const hkAabb& volume, const hkVector4& bitOffsetLow, const hkVector4& bitOffsetHigh, const hkVector4& bitScale, hkAabbTreeAabb& volumeOut);
		HK_FORCE_INLINE static void HK_CALL convertAabbToTreeSpace( const hkAabbUint32& volumeIn, const hkVector4& bitOffsetLow, const hkVector4& bitOffsetHigh, const hkVector4& bitScale,  hkAabbTreeAabb& volumeOut);
		HK_FORCE_INLINE static void HK_CALL restoreAabb(const hkAabbTreeAabb& in, const hkVector4& bitScaleInv, const hkVector4& bitOffsetLow, hkAabb& out);

	};
};

#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.inl>


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
