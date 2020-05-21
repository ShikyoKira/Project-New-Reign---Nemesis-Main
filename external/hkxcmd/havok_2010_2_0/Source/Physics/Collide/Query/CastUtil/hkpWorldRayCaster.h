/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_WORLD_RAY_CASTER_H
#define HK_WORLD_RAY_CASTER_H

#include <Physics/Collide/BroadPhase/hkpBroadPhaseCastCollector.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>

struct hkpWorldRayCastInput;
struct hkpWorldRayCastOutput;
class hkpCollisionFilter;
class hkpRayCollidableFilter;
class hkpBroadPhase;
typedef char hkpBroadPhaseAabbCache;

	/// This is a utility class, which connects the hkpBroadPhase::castRay() to the hkpShape::castRay
	/// This method uses the collector interface (instead of the structure interface ( see hkpSimpleWorldRayCaster ) )
	/// This is called by hkpWorld::castRay(). Usually you should call hkpWorld::castRay instead of
	/// using this class directly.
class hkpWorldRayCaster : public hkpBroadPhaseCastCollector
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpWorldRayCaster);
		hkpWorldRayCaster(){}

		~hkpWorldRayCaster(){}

			/// This function cast a ray through the world.
			/// For each narrowphase object the ray hits, the hkpWorldRayCastOutput receives a callback
			/// You can pass in an optional hkpBroadPhaseAabbCache: See hkpBroadPhase for Details.
		void castRay( const hkpBroadPhase& broadphase, const hkpWorldRayCastInput& input, const hkpCollisionFilter* filter, const hkpBroadPhaseAabbCache* cache, hkpRayHitCollector& collector );

			/// Casts a group of rays in close proximity
			/// collectorBase points to an array of collectors of the same type (not pointers) and
			/// collectorStriding should be set to the size of the collector type.
			///
			/// Note: If you set collectorStriding to 0, than all hits will be reported to the first collector
		void castRayGroup( const hkpBroadPhase& broadphase, const hkpWorldRayCastInput* inputArray, int numRays, const hkpCollisionFilter* filter, hkpRayHitCollector* collectorBase, int collectorStriding ) ;

			/// Cast multiple rays having the same startpoint.
			/// Notes:
			///   - the startpoint is taken only from the first input
			///   - However filtering can be done separate for each ray.
		void castRaysFromSinglePoint( const hkpBroadPhase& broadphase, const hkpWorldRayCastInput* inputArray, int numRays, const hkpCollisionFilter* filter, const hkpBroadPhaseAabbCache* cache, hkpRayHitCollector* collectorBase, int collectorStriding );

	protected:
		virtual	hkReal addBroadPhaseHandle( const hkpBroadPhaseHandle* broadPhaseHandle, int castIndex );

	protected:
		const hkpWorldRayCastInput*   m_input;
		const hkpRayCollidableFilter* m_filter;
		hkpRayHitCollector*			 m_collectorBase;
		int							 m_collectorStriding;

		// used as a temporary storage
		hkpShapeRayCastInput	m_shapeInput;
};


#endif //HK_WORLD_RAY_CASTER_H

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
