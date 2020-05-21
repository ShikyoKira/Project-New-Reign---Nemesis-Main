/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_BOX_BOX_AGENT_H
#define HK_COLLIDE2_BOX_BOX_AGENT_H

#include <Physics/Collide/Agent/Util/LinearCast/hkpIterativeLinearCastAgent.h>
#include <Physics/Collide/BoxBox/hkpBoxBoxManifold.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>


class hkpCollisionDispatcher;
	/// An hkpBoxBoxAgent handles collisions between pairs of hkBoxShapes. This produces high quality manifolds, in many cases with a good CPU
	/// Note: The implementation of this Agent handles vertex-face and edge-edge combinations, it does not handle vertex-vertex situations.
	/// That means, if you are using getClosestPoints() with a tolerance >= 0.0, the closest distance to a box might actually be outside the box.
	/// So if you are heavily relying on getClosestPoints() with long distances, you should not enable the hkpBoxBoxAgent in the hkpAgentRegisterUtil
class hkpBoxBoxAgent : public hkpIterativeLinearCastAgent
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		static void HK_CALL initAgentFunc(hkpCollisionDispatcher::AgentFuncs& f);

			/// Registers this agent with the collision dispatcher.
		static void HK_CALL registerAgent(hkpCollisionDispatcher* dispatcher);

			// hkpCollisionAgent interface implementation.
		virtual void processCollision(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result);

			// Deprecated. Use hkpGskBaseAgent::getClosestPoints instead.
		virtual void getClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector ) ;

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector ) ;

			// hkpCollisionAgent interface implementation.
		virtual void getPenetrations( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetPenetrations( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		virtual void cleanup(hkCollisionConstraintOwner& info);

			/// Setting this true forces the box-box detection to try all edge combinations for additional contact points immediately after one contact point is found.
			/// This improves the fidelity of the simulation at the cost of CPU cycles. Defaults to false.
		inline static void HK_CALL setAttemptToFindAllEdges( hkBool findEm )
		{
			m_attemptToFindAllEdges = findEm;
		}

			/// If this is true, it forces the box-box detection to try all edge combinations for additional contact points immediately after one contact point is found.
			/// This improves the fidelity of the simulation at the cost of CPU cycles. Defaults to false.
		inline static hkBool HK_CALL getAttemptToFindAllEdges()
		{
			return m_attemptToFindAllEdges;
		}

	protected:
			/// Called by createBoxBoxAgent().
		HK_FORCE_INLINE hkpBoxBoxAgent( hkpContactMgr* contactMgr ):  hkpIterativeLinearCastAgent(contactMgr){}

			/// Agent creation function used by the hkpCollisionDispatcher.
		static hkpCollisionAgent* HK_CALL createBoxBoxAgent(	const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* contactMgr);


	protected:
		hkpBoxBoxManifold m_manifold;

		static hkBool m_attemptToFindAllEdges;
};

#endif // HK_COLLIDE2_BOX_BOX_AGENT_H

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
