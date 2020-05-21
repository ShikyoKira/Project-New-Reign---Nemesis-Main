/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MIN_DISTANCE_COLLISION_INFO_COLLECTOR_H
#define HK_MIN_DISTANCE_COLLISION_INFO_COLLECTOR_H

#include <Physics/Collide/Shape/Query/hkpRayHitCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

/// hkpClosestRayHitCollector collects the closest contact point and information about a cast along a linear path.
/// Both the closest contact point for non-penetrating (cast) and for penetrating cases are collected.
class hkpClosestRayHitCollector : public hkpRayHitCollector
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpClosestRayHitCollector);
			/// Constructor calls reset.
		inline hkpClosestRayHitCollector();

			/// Resets this structure if you want to reuse it for another raycast.
		inline void reset();

		inline virtual ~hkpClosestRayHitCollector();
		
			/// returns true, if this class has collected a hit
		inline hkBool hasHit( ) const;

			/// returns the full hit information
		inline const hkpWorldRayCastOutput& getHit() const;

	protected:

		virtual void addRayHit( const hkpCdBody& cdBody, const hkpShapeRayCastCollectorOutput& hitInfo );
	
	protected:
		hkpWorldRayCastOutput	m_rayHit;
};

#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.inl>

#endif //HK_MIN_DISTANCE_COLLISION_INFO_COLLECTOR_H

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
