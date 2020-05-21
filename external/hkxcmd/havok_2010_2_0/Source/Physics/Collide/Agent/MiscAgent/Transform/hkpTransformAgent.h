/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_TRANSFORM_AGENT_H
#define HK_COLLIDE2_TRANSFORM_AGENT_H

#include <Physics/Collide/Agent/hkpCollisionAgent.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>

class hkpCollisionDispatcher;

	/// This agent handles collisions between hkpTransformShape and other shapes.
class hkpTransformAgent : public hkpCollisionAgent
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Registers this agent with the collision dispatcher.
		static void HK_CALL registerAgent(hkpCollisionDispatcher* dispatcher);

			// hkpCollisionAgent interface implementation.
		virtual void processCollision(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result);

			// hkpCollisionAgent interface implementation.
		virtual void linearCast( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpLinearCastCollisionInput& input, hkpCdPointCollector& collector, hkpCdPointCollector* startCollector );

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticLinearCast( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpLinearCastCollisionInput& input, hkpCdPointCollector& collector, hkpCdPointCollector* startCollector );

			// hkpCollisionAgent interface implementation.
		virtual void getClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdPointCollector& pointDetails);

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector  );

			// hkpCollisionAgent interface implementation.
		virtual void getPenetrations( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetPenetrations( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		virtual void cleanup( hkCollisionConstraintOwner& constraintOwner );

			// hkpCollisionAgent interface implementation.
		virtual void invalidateTim( const hkpCollisionInput& input);

			// hkpCollisionAgent interface implementation.
		virtual void warpTime(hkTime oldTime, hkTime newTime, const hkpCollisionInput& input);

		// hkpCollisionAgent interface implementation.
		virtual void removePoint( hkContactPointId idToRemove );

		// hkpCollisionAgent interface implementation.
		virtual void commitPotential( hkContactPointId newId );

		// hkpCollisionAgent interface implementation.
		virtual void createZombie( hkContactPointId idTobecomeZombie );

        void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const;

	protected:
		HK_FORCE_INLINE hkpTransformAgent(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr);

		virtual void updateShapeCollectionFilter( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkCollisionConstraintOwner& constraintOwner );

			/// Agent creation function used by the hkpCollisionDispatcher.
		static hkpCollisionAgent* HK_CALL createTransformAAgent(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr);
		static hkpCollisionAgent* HK_CALL createTransformBAgent(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr);

	protected:
		hkpCollisionAgent*	m_childAgent;
};



#endif // HK_COLLIDE2_TRANSFORM_AGENT_H

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
