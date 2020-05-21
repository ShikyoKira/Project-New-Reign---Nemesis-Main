/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ALL_RAY_HIT_COLLECTOR_H
#define HK_ALL_RAY_HIT_COLLECTOR_H

#include <Physics/Collide/Shape/Query/hkpRayHitCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>

/// This class implements the hkpRayHitCollector, and stores all the points the ray hits.
/// It never causes an early out for the ray cast.
class hkpAllRayHitCollector : public hkpRayHitCollector
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpAllRayHitCollector);
			/// Constructor calls reset.
		inline hkpAllRayHitCollector();

			/// Resets this structure by clearing the hits list, and resetting the early out distance
			/// You must call this if you want to reuse an object of this class for another raycast.
		inline void reset();

		inline virtual ~hkpAllRayHitCollector();

			/// Get all the hits
		inline const hkArray<hkpWorldRayCastOutput>& getHits() const;

			/// Sort the hits so that the closest hits occur earliest in the hit list
		void sortHits(); 

	protected:

		virtual void addRayHit( const hkpCdBody& cdBody, const hkpShapeRayCastCollectorOutput& hitInfo );

	protected:
		hkInplaceArray<hkpWorldRayCastOutput,8> m_hits;
};

#include <Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.inl>


#endif //HK_ALL_RAY_HIT_COLLECTOR_H

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
