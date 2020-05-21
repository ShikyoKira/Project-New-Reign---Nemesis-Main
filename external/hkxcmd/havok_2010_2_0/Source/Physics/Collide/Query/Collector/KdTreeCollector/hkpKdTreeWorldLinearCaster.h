/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_KD_TREE_WORLD_LINEAR_CASTER
#define HKP_KD_TREE_WORLD_LINEAR_CASTER

#include <Common/Internal/KdTree/QueryUtils/hkKdTreeCastCollector.h>
#include <Physics/Collide/Shape/hkpShapeType.h>
#include <Physics/Collide/Agent/Query/hkpLinearCastCollisionInput.h>

struct hkpCollisionInput;
struct hkpLinearCastInput;
struct hkpCollisionAgentConfig;
class hkpCdPointCollector;
class hkpCollisionFilter;
class hkpCollidableCollidableFilter;
class hkpCollidable;

	/// This is a utility class you can use to perform a linear cast with a collidable against all other collidables in the broad
	/// phase. It has one function, linear cast.
	/// It effectively connects hkKdTree::castAabb with hkpCollisionAgent::linearCast
	/// This is called by hkpWorld::linearCast() by default. Usually you should call hkpWorld::linearCast instead of
	/// using this class directly.
class hkpKdTreeWorldLinearCaster : public hkKdTreeCastCollector
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpKdTreeWorldLinearCaster);
		hkpKdTreeWorldLinearCaster(){}

		~hkpKdTreeWorldLinearCaster(){}

			/// Inputs are:
			/// - a reference to the kd tree, and it's offset and scale vectors
			/// - the collidable to linear cast
			/// - the linear cast input
			/// - the collidable-collidable filter
			/// - the hkpCollisionInput (for the narrowphase linear casts)
			/// - collectors for start points and cast points
			/// - an optional an hkpBroadPhaseAabbCache: See hkpBroadPhase for Details or HK_NULL if you don't want to use it
			/// For each narrowphase linear cast hit, the collectors will receive a callback.
		void linearCast( const hkKdTree* tree, const hkpCollidable* collA, 
					     const hkpLinearCastInput& input, const hkpCollidableCollidableFilter* filter,
						 const hkpCollisionInput& collInput, hkpCollisionAgentConfig* config,
						 hkpCdPointCollector& castCollector, hkpCdPointCollector* startPointCollector );

	protected:
		virtual	hkBool addMidPhaseHit( hkPrimitiveId primId );
		virtual hkBool hasHit();
		virtual hkReal getHitFraction();

	protected:

		const hkpLinearCastInput* m_input;
		const hkpCollidableCollidableFilter* m_filter;
		hkpCdPointCollector*			 m_castCollector;
		hkpCdPointCollector*			 m_startPointCollector;
		const hkpCollidable*			 m_collidableA;
		hkEnum<hkpShapeType,int>		 m_typeA;

		// used as a temporary storage
		hkpLinearCastCollisionInput	m_shapeInput;
};


#endif //HK_BROAD_PHASE_LINEAR_CASTER

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
